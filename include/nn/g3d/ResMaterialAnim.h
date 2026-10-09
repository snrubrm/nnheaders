/**
 * @file ResMaterialAnim.h
 * @brief Resource file for material animations.
 */

#pragma once

#include <nn/types.h>
#include <nn/g3d/TextureRef.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace g3d {

class ResMaterialAnim {
public:
    void ReleaseTexture();
    s32 BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void Reset();

    // Native file arrays advance by 78; BindTexture reads the final u16 at 76.
    u8 _0[0x38];
    const gfx::TextureView** mTextureViews;  // 38
    nn::util::BinPtrToString* mTextureNames;  // 40
    u8 _48[0x10];
    gfx::DescriptorSlot* mTextureDescriptorSlots;  // 58
    u8 _60[0x16];
    u16 mTextureCount;  // 76
};
}  // namespace g3d
}  // namespace nn
