/**
 * @file ResModel.h
 * @brief Resource model.
 */

#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>
#include <nn/g3d/TextureRef.h>

namespace nn {
namespace g3d {
class ResMaterial;


class ResModel {
public:
    u64 BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(u32);
    nn::g3d::ResMaterial* FindMaterial(char const* materialName) const;

    // BindTexture ac6308 reads material count 6c and B8-byte array at 40.
    // Native ResFile arrays advance by 78 in ac57fc/ac58a0.
    u8 _0[0x40];
    ResMaterial* mMaterials;
    u8 _48[0x6c - 0x48];
    u16 mMaterialCount;
    u8 _6e[0x78 - 0x6e];
};
}  // namespace g3d
}  // namespace nn
