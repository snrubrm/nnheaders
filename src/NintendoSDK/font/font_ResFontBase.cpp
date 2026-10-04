#include <nn/font/font_ResFont.h>

namespace nn::font {

// NON_MATCHING: the compiler loads the sheet data before the resource base for the byte offset calculation.
// 0x7101326d3c
void ResFontBase::GenTextureNames(gfx::Device* device) {
    if (mTexture.IsSet())
        return;
    const FontTextureGlyph* glyph = GetTextureGlyph();
    const u8 sheet_count = GetActiveSheetCount();
    mTexture.Set(GetPointer<void>(mFileHeader, glyph->sheetImage), glyph->sheetFormat,
                 glyph->sheetWidth, glyph->sheetHeight, sheet_count, true);
    const u16 format = mTexture.GetFormat() & 0x3fff;
    if (format > 19 || format == 1 || format == 6 || format == 7)
        return;
    const s64 offset = static_cast<const u8*>(mTexture.GetData()) - static_cast<const u8*>(mResource);
    mTexture.Initialize(device, mMemoryPool, mPoolOffset + offset, mPoolSize);
}

// 0x71013265d8
s32 ResFontBase::GetWidth() const {
    return mFontInfo->width;
}

// 0x71013265e4
s32 ResFontBase::GetHeight() const {
    return mFontInfo->height;
}

// 0x71013265f0
s32 ResFontBase::GetAscent() const {
    return mFontInfo->ascent;
}

// 0x71013265fc
s32 ResFontBase::GetDescent() const {
    return mFontInfo->height - mFontInfo->ascent;
}

// 0x7101326610
s32 ResFontBase::GetBaselinePos() const {
    return GetTextureGlyph()->baselinePos;
}

// 0x710132662c
s32 ResFontBase::GetCellHeight() const {
    return GetTextureGlyph()->cellHeight;
}

// 0x7101326648
s32 ResFontBase::GetCellWidth() const {
    return GetTextureGlyph()->cellWidth;
}

// 0x7101326664
s32 ResFontBase::GetMaxCharWidth() const {
    return GetTextureGlyph()->maxCharWidth;
}

// 0x7101326680
Font::Type ResFontBase::GetType() const {
    return Type_Resource;
}

// 0x7101326688
u32 ResFontBase::GetTextureFormat() const {
    return GetTextureGlyph()->sheetFormat & 0x3fff;
}

// 0x71013266a8
s32 ResFontBase::GetLineFeed() const {
    return mFontInfo->lineFeed;
}

// 0x71013266b4
CharWidths ResFontBase::GetDefaultCharWidths() const {
    return mFontInfo->defaultWidth;
}

// 0x71013266c8
void ResFontBase::SetDefaultCharWidths(const CharWidths& widths) {
    mFontInfo->defaultWidth = widths;
}

// 0x710132688c
void ResFontBase::SetLineFeed(s32 line_feed) {
    mFontInfo->lineFeed = static_cast<s8>(line_feed);
}

// 0x7101326c70
u32 ResFontBase::GetCharacterCode() const {
    return mFontInfo->encoding;
}

// 0x7101326cc0
void ResFontBase::SetLinearFilterEnabled(bool at_small, bool at_large) {
    mTextureWrapFilter = (at_large << 1) | (at_small << 2);
}

// 0x7101326cd8
bool ResFontBase::IsLinearFilterEnabledAtSmall() const {
    return (mTextureWrapFilter >> 2) & 1;
}

// 0x7101326ce4
bool ResFontBase::IsLinearFilterEnabledAtLarge() const {
    return (mTextureWrapFilter >> 1) & 1;
}

// 0x7101326cf0
u32 ResFontBase::GetTextureWrapFilterValue() const {
    return mTextureWrapFilter;
}

// 0x7101326cf8
bool ResFontBase::IsColorBlackWhiteInterpolationEnabled() const {
    return mTexture.IsColorBlackWhiteInterpolationEnabled();
}

// 0x7101326d00
void ResFontBase::SetColorBlackWhiteInterpolationEnabled(bool enabled) {
    mTexture.SetColorBlackWhiteInterpolationEnabled(enabled);
}

// 0x7101326d0c
bool ResFontBase::IsBorderEffectEnabled() const {
    return mFontInfo->fontType == 2;
}

// 0x7101326d20
u8 ResFontBase::GetActiveSheetCount() const {
    return GetTextureGlyph()->sheetCount;
}

// 0x7101326584
void ResFontBase::SetResourceBuffer(void* resource, FontInformation* font_info,
                                    FontKerningTable* kerning, gfx::MemoryPool* pool, s64 pool_offset,
                                    u64 pool_size) {
    mResource = resource;
    mFontInfo = font_info;
    mKerningTable = kerning;
    mMemoryPool = pool;
    mPoolOffset = pool_offset;
    mPoolSize = pool_size;
}

// 0x7101326598
void* ResFontBase::RemoveResourceBuffer() {
    void* resource = mResource;
    mResource = nullptr;
    mFontInfo = nullptr;
    mKerningTable = nullptr;
    mMemoryPool = nullptr;
    mPoolOffset = 0;
    mPoolSize = 0;
    return resource;
}

// 0x7101326c7c
void* ResFontBase::FindBlock(detail::BinaryFileHeader* header, u32 kind) {
    u8* block = reinterpret_cast<u8*>(header) + header->headerSize;
    for (s32 i = 0; i < header->dataBlocks; i++) {
        auto* block_header = reinterpret_cast<detail::BinaryBlockHeader*>(block);
        if (block_header->kind == kind)
            return block_header + 1;
        block += block_header->size;
    }
    return nullptr;
}

// 0x7101326720
u16 ResFontBase::FindGlyphIndex(u32 code) const {
    const FontCodeMap* map = GetPointer<FontCodeMap>(mFileHeader, mFontInfo->pMap);
    while (map) {
        if (map->ccodeBegin <= code && code <= map->ccodeEnd)
            break;
        map = GetPointer<FontCodeMap>(mFileHeader, map->pNext);
    }
    if (!map)
        return 0xffff;

    if (mRangeCount != 0) {
        for (s32 i = 0;; i++) {
            if (i >= mRangeCount)
                return 0xffff;
            if (mRangeBegin[i] <= code && code <= mRangeEnd[i])
                break;
        }
    }

    switch (map->mappingMethod) {
    case FontCodeMap::MappingMethod_Direct:
        return code - map->ccodeBegin + map->mapInfo[0];
    case FontCodeMap::MappingMethod_Table:
        return map->mapInfo[s32(code - map->ccodeBegin)];
    case FontCodeMap::MappingMethod_Scan: {
        const auto* scan = reinterpret_cast<const FontCodeMapScan*>(map->mapInfo);
        const FontCodeMapScanEntry* first = scan->entries;
        const FontCodeMapScanEntry* last = scan->entries + scan->count - 1;
        while (first <= last) {
            const FontCodeMapScanEntry* mid = first + (last - first) / 2;
            if (mid->ccode < code)
                first = mid + 1;
            else if (mid->ccode > code)
                last = mid - 1;
            else
                return mid->index;
        }
        return 0xffff;
    }
    default:
        return 0xffff;
    }
}

// NON_MATCHING: same stores and arithmetic; the original loads the sheet row / line before the character index is
// widened, and computes the cell position values before its stores (scheduling only)
// 0x71013269a0
void ResFontBase::GetGlyphFromIndex(Glyph* glyph, u16 index) const {
    const FontTextureGlyph* texture_glyph = GetTextureGlyph();

    const u32 sheet_index = index / (texture_glyph->sheetRow * texture_glyph->sheetLine);
    glyph->pTexture =
        GetPointer<u8>(mFileHeader, texture_glyph->sheetImage) + texture_glyph->sheetSize * sheet_index;

    const CharWidths& widths = GetCharWidthsFromIndex(index);
    glyph->leftWidth = widths.left;
    glyph->glyphWidth = widths.glyphWidth;
    glyph->charWidth = widths.charWidth;
    glyph->_e = widths.glyphWidth;
    glyph->pTextureObject = &mTexture;
    glyph->sheetIndex = sheet_index;
    glyph->_1e = 0;
    glyph->_28 = 0;

    const u32 cell_index = index % (texture_glyph->sheetRow * texture_glyph->sheetLine);
    const u32 line = cell_index / texture_glyph->sheetRow;
    const u32 row = cell_index % texture_glyph->sheetRow;
    glyph->height = texture_glyph->cellHeight;
    glyph->_12 = texture_glyph->cellHeight;
    glyph->texFormat = texture_glyph->sheetFormat & 0x3fff;
    glyph->texWidth = texture_glyph->sheetWidth;
    glyph->texHeight = texture_glyph->sheetHeight;
    glyph->cellX = (texture_glyph->cellWidth + 1) * row + 1;
    glyph->cellY = (texture_glyph->cellHeight + 1) * line + 1;
}

// 0x71013268bc
CharWidths ResFontBase::GetCharWidths(u32 code) const {
    u16 index = FindGlyphIndex(code);
    if (index == 0xffff)
        index = mFontInfo->alterCharIndex;
    return GetCharWidthsFromIndex(index);
}

// 0x7101326954
void ResFontBase::GetGlyph(Glyph* glyph, u32 code) const {
    u16 index = FindGlyphIndex(code);
    if (index == 0xffff)
        index = mFontInfo->alterCharIndex;
    GetGlyphFromIndex(glyph, index);
}

// 0x71013266e0
bool ResFontBase::SetAlternateChar(u32 code) {
    const u16 index = FindGlyphIndex(code);
    if (index == 0xffff)
        return false;
    mFontInfo->alterCharIndex = index;
    return true;
}

// 0x710132689c
s32 ResFontBase::GetCharWidth(u32 code) const {
    return GetCharWidths(code).charWidth;
}

// 0x7101326ad8
bool ResFontBase::HasGlyph(u32 code) const {
    return FindGlyphIndex(code) != 0xffff;
}

// 0x71013265b4
void ResFontBase::RegisterTextureViewToDescriptorPool(
    bool (*register_function)(gfx::DescriptorSlot*, const gfx::TextureView&, void*), void* user_data) {
    register_function(mTexture.GetDescriptorSlot(), *mTexture.GetTextureView(), user_data);
}

// 0x71013263d0
ResFontBase::ResFontBase()
    : mResource(nullptr), mFontInfo(nullptr), mKerningTable(nullptr), mMemoryPool(nullptr),
      mPoolOffset(0), mPoolSize(0), mRangeCount(0) {
    SetLinearFilterEnabled(true, true);
}

// 0x71013264ac
ResFontBase::~ResFontBase() {}

}  // namespace nn::font
