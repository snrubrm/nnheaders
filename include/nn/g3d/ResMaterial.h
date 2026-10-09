/**
 * @file ResMaterial.h
 * @brief Resource material for models.
 */

#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>
#include <nn/g3d/TextureRef.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace g3d {

class ResMaterial {
public:
    u64 BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(u32);

    // Native BindTexture ac895c and AGL bindings b33f24/b33f50.
    u8 _0[0x30];
    const gfx::TextureView** mTextureViews;  // 30
    nn::util::BinPtrToString* mTextureNames;  // 38
    u8 _40[0x98 - 0x40];
    gfx::DescriptorSlot* mTextureDescriptorSlots;  // 98
    u8 _a0[8];
    u8 mTextureCount;  // a8
    u8 _a9[0xb4 - 0xa9];
};
}  // namespace g3d
}  // namespace nn
