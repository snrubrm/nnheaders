/**
 * @file font_GpuBuffer.h
 * @brief CPU-writable buffer that the font and ui2d renderers allocate constant and vertex data from.
 */

#pragma once

#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>

namespace nn::ui2d {
class Material;
}

namespace nn::font {

// Partial layout (0x40 bytes): eui::ConstantBuffer embeds two of these at +0x120 / +0x160 and its map()
// (0x7100bf42f8) is the only caller of Map / Unmap (0x7101324c88 / 0x7101324ce0). Map selects element `index` of the
// 0x48-byte gfx buffer array at +0x8 and caches the mapped pointer at +0x30; Unmap undoes that and resets the mapped
// index at +0x24 to -1. Member names are guesses.
class GpuBuffer {
public:
    void Map(s32 index);
    void Unmap();

    // inline-only in the original; name is a guess. eui::ConstantBuffer::map stores the frame's buffer index here
    // between Unmap and Map.
    void SetBufferIndex(s32 index) { mBufferIndex = index; }

private:
    // Material constant-buffer accessors 0x7100ac391c / 0x7100ac4ab0 read the mapped pointer.
    friend class nn::ui2d::Material;

    /* 0x00 */ u8 mFlags;
    /* 0x08 */ gfx::Buffer* mBuffers;
    /* 0x10 */ u8 _10[0x24 - 0x10];
    /* 0x24 */ s32 mMappedIndex;
    /* 0x28 */ s32 mBufferIndex;
    /* 0x30 */ void* mMappedPtr;
    /* 0x38 */ void* _38;
};
static_assert(sizeof(GpuBuffer) == 0x40);

}  // namespace nn::font
