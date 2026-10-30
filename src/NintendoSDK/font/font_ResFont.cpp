#include <nn/font/font_ResFont.h>
#include <nn/util.h>
#include <nn/util/util_BinTypes.h>
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_BytePtr.h>

namespace nn::font {

// 0x7101325fdc
ResFont::ResFont() {}

// The original keeps the vtable store (a plain empty body drops it); `{ ; }` as in upstream's
// GameDataFlagSelector::~GameDataFlagSelector (commit 96101229).
// 0x710132600c
ResFont::~ResFont() {
    ;
}

// NON_MATCHING: major-version comparison and initialization scheduling differ.
// 0x7101326090
bool ResFont::SetResource(gfx::Device* device, void* resource, gfx::MemoryPool* pool,
                           s64 pool_offset, u64 pool_size) {
    if (mResource)
        return false;
    auto* header = static_cast<detail::BinaryFileHeader*>(resource);
    const u32 signature = header->signature;
    if (signature != util::MakeSignature('F', 'F', 'N', 'T')) {
        char message[256];
        // The native failure path formats this diagnostic before returning.
        util::SNPrintf(message, sizeof(message),
                       "Signature check failed ('%c%c%c%c' must be '%c%c%c%c').",
                       u8(signature >> 24), u8(signature >> 16), u8(signature >> 8),
                       u8(signature), 'T', 'N', 'F', 'F');
        return false;
    }
    if (header->byteOrder != 0xfeff)
        return false;
    const u32 version = header->version;
    if ((version >> 24) != 4 || ((version >> 16) & 0xff) > 1)
        return false;
    if (header->fileSize < 0x24 || header->dataBlocks < 2)
        return false;

    FontInformation* font_info = nullptr;
    auto* block = util::BytePtr(header, header->headerSize).Get<detail::BinaryBlockHeader>();
    for (s32 i = 0; i < header->dataBlocks; ++i) {
        switch (block->kind) {
        case util::MakeSignature('F', 'I', 'N', 'F'):
            font_info = util::BytePtr(block, sizeof(*block)).Get<FontInformation>();
            break;
        case util::MakeSignature('K', 'R', 'N', 'G'):
        case util::MakeSignature('C', 'W', 'D', 'H'):
        case util::MakeSignature('C', 'M', 'A', 'P'):
        case util::MakeSignature('T', 'G', 'L', 'P'):
        case util::MakeSignature('G', 'L', 'G', 'R'):
            break;
        default:
            mFileHeader = header;
            return false;
        }
        block = util::BytePtr(block, block->size).Get<detail::BinaryBlockHeader>();
    }
    mFileHeader = header;
    if (!font_info)
        return false;
    auto* kerning = static_cast<FontKerningTable*>(
        FindBlock(header, util::MakeSignature('K', 'R', 'N', 'G')));
    SetResourceBuffer(resource, font_info, kerning, pool, pool_offset, pool_size);
    GenTextureNames(device);
    return true;
}

// NON_MATCHING: the compiler removes the first null-offset selection before dereferencing the glyph.
// 0x710132627c
void ResFont::Unrelocate(void* resource) {
    auto* header = static_cast<detail::BinaryFileHeader*>(resource);
    auto* block = util::BytePtr(header, header->headerSize).Get<detail::BinaryBlockHeader>();
    if (block->kind != util::MakeSignature('F', 'I', 'N', 'F')) {
        for (;;) {
        }
    }
    auto* font_info = util::BytePtr(block, sizeof(*block)).Get<FontInformation>();
    FontTextureGlyph* glyph;
    if (font_info->pGlyph == 0)
        glyph = nullptr;
    else
        glyph = util::BytePtr(header, font_info->pGlyph).Get<FontTextureGlyph>();
    util::BinaryFileHeader* texture_header;
    if (glyph->sheetImage == 0)
        texture_header = nullptr;
    else
        texture_header = util::BytePtr(header, glyph->sheetImage).Get<util::BinaryFileHeader>();
    if (texture_header->IsRelocated()) {
        texture_header->GetRelocationTable()->Unrelocate();
        texture_header->SetRelocated(false);
    }
}

void ResFont::Finalize(gfx::Device* device) {
    if (mResource) {
        UnloadTexture(device);
        RemoveResourceBuffer();
    }
}

}  // namespace nn::font
