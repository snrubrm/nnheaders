/**
 * @file font_ResFont.h
 * @brief Fonts loaded from a binary font resource (BFFNT).
 */

#pragma once

#include <nn/font/font_Font.h>
#include <nn/font/font_TextureObject.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::font {

namespace detail {
// Header of a binary font file (ResFont::SetResource checks the signature 'FFNT' and the byte order marker, walks
// `dataBlocks` blocks starting at `headerSize`; ResFontBase::FindBlock).
struct BinaryFileHeader {
    u32 signature;
    u16 byteOrder;
    u16 headerSize;
    u32 version;
    u32 fileSize;
    u16 dataBlocks;
    u16 reserved;
};
static_assert(sizeof(BinaryFileHeader) == 0x14);

struct BinaryBlockHeader {
    u32 kind;
    u32 size;
};
}  // namespace detail

// Offsets inside the font data are relative to the file header (ResFontBase::mFileHeader).
struct FontInformation {
    u8 fontType;  // 2: border effect (IsBorderEffectEnabled)
    u8 height;
    u8 width;
    u8 ascent;
    s16 lineFeed;
    u16 alterCharIndex;
    CharWidths defaultWidth;
    u8 encoding;
    u32 pGlyph;
    u32 pWidth;
    u32 pMap;
};
static_assert(sizeof(FontInformation) == 0x18);

// The glyph sheets (texture glyph block).
struct FontTextureGlyph {
    u8 cellWidth;
    u8 cellHeight;
    u8 sheetCount;
    u8 maxCharWidth;
    u32 sheetSize;
    s16 baselinePos;
    u16 sheetFormat;  // low 14 bits: the texture format
    u16 sheetRow;
    u16 sheetLine;
    u16 sheetWidth;
    u16 sheetHeight;
    u32 sheetImage;
};
static_assert(sizeof(FontTextureGlyph) == 0x18);

// A range of glyph indices with their character widths (a chain of them, linked by `pNext`).
struct FontWidth {
    u16 indexBegin;
    u16 indexEnd;
    u32 pNext;
    CharWidths widthTable[1];
};

// Mapping from character codes to glyph indices (a chain of ranges, linked by `pNext`); the data of the range follows
// the header: `mappingMethod` 0 (direct) has one u16 index offset, 1 (table) one u16 index per code, 2 (scan) a
// FontCodeMapScan.
struct FontCodeMap {
    enum MappingMethod {
        MappingMethod_Direct,
        MappingMethod_Table,
        MappingMethod_Scan,
    };

    u32 ccodeBegin;
    u32 ccodeEnd;
    u16 mappingMethod;
    u16 reserved;
    u32 pNext;
    u16 mapInfo[1];
};

struct FontCodeMapScanEntry {
    u32 ccode;
    u16 index;
    u16 reserved;
};

struct FontCodeMapScan {
    u16 count;
    u16 reserved;
    FontCodeMapScanEntry entries[1];
};

struct FontKerningTable;

class ResFontBase : public Font {
public:
    NN_RUNTIME_TYPEINFO(Font)

    ResFontBase();
    ~ResFontBase() override;

    s32 GetWidth() const override;
    s32 GetHeight() const override;
    s32 GetAscent() const override;
    s32 GetDescent() const override;
    s32 GetMaxCharWidth() const override;
    Type GetType() const override;
    u32 GetTextureFormat() const override;
    s32 GetLineFeed() const override;
    CharWidths GetDefaultCharWidths() const override;
    void SetLineFeed(s32 line_feed) override;
    void SetDefaultCharWidths(const CharWidths& widths) override;
    bool SetAlternateChar(u32 code) override;
    s32 GetCharWidth(u32 code) const override;
    CharWidths GetCharWidths(u32 code) const override;
    void GetGlyph(Glyph* glyph, u32 code) const override;
    bool HasGlyph(u32 code) const override;
    s32 GetKerning(u32 first, u32 second) const override;
    u32 GetCharacterCode() const override;
    s32 GetBaselinePos() const override;
    s32 GetCellHeight() const override;
    s32 GetCellWidth() const override;
    void SetLinearFilterEnabled(bool at_small, bool at_large) override;
    bool IsLinearFilterEnabledAtSmall() const override;
    bool IsLinearFilterEnabledAtLarge() const override;
    u32 GetTextureWrapFilterValue() const override;
    bool IsColorBlackWhiteInterpolationEnabled() const override;
    void SetColorBlackWhiteInterpolationEnabled(bool enabled) override;
    bool IsBorderEffectEnabled() const override;
    // New virtual of ResFontBase (the last slot of its vtable)
    virtual u8 GetActiveSheetCount() const;

    void SetResourceBuffer(void* resource, FontInformation* font_info, FontKerningTable* kerning,
                           gfx::MemoryPool* pool, s64 pool_offset, u64 pool_size);
    void* RemoveResourceBuffer();
    void GenTextureNames(gfx::Device* device);
    void RegisterTextureViewToDescriptorPool(
        bool (*register_function)(gfx::DescriptorSlot*, const gfx::TextureView&, void*),
        void* user_data);

    // The glyph index of a character code (0xffff: none).
    u16 FindGlyphIndex(u32 code) const;
    void GetGlyphFromIndex(Glyph* glyph, u16 index) const;
    // inline-only in the original (GetCharWidths and GetGlyphFromIndex run the same chain walk); name is a guess
    const CharWidths& GetCharWidthsFromIndex(u16 index) const {
        for (const FontWidth* width = GetPointer<FontWidth>(mFileHeader, mFontInfo->pWidth); width;
             width = GetPointer<FontWidth>(mFileHeader, width->pNext)) {
            if (width->indexBegin <= index && index <= width->indexEnd)
                return width->widthTable[index - width->indexBegin];
        }
        return mFontInfo->defaultWidth;
    }

    static void* FindBlock(detail::BinaryFileHeader* header, u32 kind);

protected:
    // Offsets in the font data are relative to the file header; 0 means "none". (An if / else with the operands as
    // arguments, not a ternary: the null arm has to survive as a select, as in the original.)
    template <typename T>
    static const T* GetPointer(const detail::BinaryFileHeader* header, u32 offset) {
        const T* pointer;
        if (offset == 0)
            pointer = nullptr;
        else
            pointer = reinterpret_cast<const T*>(reinterpret_cast<const u8*>(header) + offset);
        return pointer;
    }

    const FontTextureGlyph* GetTextureGlyph() const {
        return GetPointer<FontTextureGlyph>(mFileHeader, mFontInfo->pGlyph);
    }

    /* 0x10 */ void* mResource;
    /* 0x18 */ detail::BinaryFileHeader* mFileHeader;
    /* 0x20 */ FontInformation* mFontInfo;
    /* 0x28 */ ResourceTextureObject mTexture;
    /* 0x58 */ u32 mTextureWrapFilter;  // bit 2: linear filter at small sizes, bit 1: at large sizes
    /* 0x60 */ FontKerningTable* mKerningTable;
    /* 0x68 */ gfx::MemoryPool* mMemoryPool;
    /* 0x70 */ s64 mPoolOffset;
    /* 0x78 */ u64 mPoolSize;
    /* 0x80 */ s32 mRangeCount;
    /* 0x84 */ u32 mRangeBegin[4];
    /* 0x94 */ u32 mRangeEnd[4];
};
static_assert(sizeof(ResFontBase) == 0xa8);

class ResFont : public ResFontBase {
public:
    NN_RUNTIME_TYPEINFO(ResFontBase)

    ResFont();
    ~ResFont() override;
};

}  // namespace nn::font
