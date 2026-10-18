/**
 * @file ResModel.h
 * @brief Resource model.
 */

#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>
#include <nn/g3d/TextureRef.h>
#include <nn/util/util_BinTypes.h>
#include <nn/util/util_ResDic.h>

namespace nn {
namespace g3d {
class ResMaterial;
class ResSkeleton;


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

    // Inline-only in the original; name is a guess. Independent C6F3CC and
    // C6F58C read the resource model name from this binary string.
    const char* GetName() const { return pName.Get()->GetData(); }

    // Emitted out of line in the game (FindMaterialIndex 0x7101141290) with ResDic::FindIndex inlined.
    int FindMaterialIndex(const char* name) const {
        const util::ResDic* dict = pMaterialDic.Get();
        if (dict == nullptr)
            return util::ResDic::Npos;
        return dict->FindIndex(name);
    }

    int FindShapeIndex(const char* name) const {
        const util::ResDic* dict = pShapeDic.Get();
        if (dict == nullptr)
            return util::ResDic::Npos;
        return dict->FindIndex(name);
    }

    // BindTexture AC6308 reads material count 6c and B8-byte array at 40.
    // Native ResFile arrays advance by 78 in AC57FC/AC58A0.
    u8 _0[0x10];
    nn::util::BinPtrToString pName;
    u8 _18[0x20 - 0x18];
    // Model initialization 132e0dc passes this resource to the named
    // SkeletonObj::Initialize 1337c1c, which stores it as its ResSkeleton.
    nn::util::BinTPtr<ResSkeleton> pSkeleton;
    u8 _28[0x30 - 0x28];
    nn::util::BinTPtr<nn::util::ResDic> pShapeDic;
    u8 _38[0x40 - 0x38];
    ResMaterial* mMaterials;
    nn::util::BinTPtr<nn::util::ResDic> pMaterialDic;
    u8 _50[0x6c - 0x50];
    u16 mMaterialCount;
    u8 _6e[0x78 - 0x6e];
};
}  // namespace g3d
}  // namespace nn
