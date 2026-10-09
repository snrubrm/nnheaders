/**
 * @file ResSkeletalAnim.h
 * @brief Resource file for skeletal animations.
 */

#pragma once

#include <nn/types.h>
#include <nn/nn_BitTypes.h>
#include <nn/g3d/ResBoneAnim.h>
#include <nn/util/util_BinTypes.h>

namespace nn::g3d {
class ResSkeletalAnim {
public:
    void Reset();

    // Inline-only, name follows ResBone. ModelResource initialize c0b058
    // hashes this binary string; sword-blur 11d4780 independently formats it.
    const char* GetName() const { return mName.Get()->GetData(); }

    // Inline-only in the original; names are reconstruction guesses. The signed
    // frame count at +0x4c and loop bit2 at +0x48 are read by both SkeltalAsset's
    // constructor (0x710125c4b4/0x710125c4fc) and SDK SetResource (0x710133330c).
    s32 GetFrameCount() const { return mFrameCount; }
    bool IsLooped() const { return (mFlags & 4) != 0; }

    // Inline-only in the original; names are guesses following ResSkeleton's
    // resource API. SDK Reset ac720c, SetResource 13332e0 and AS lookup
    // 115d55c independently establish array +30 and count +58.
    int GetBoneAnimCount() const { return mBoneAnimCount; }
    const ResBoneAnim* GetBoneAnim(int index) const { return &mBoneAnims[index]; }
    // Existing ResBone/ResSkeleton GetRotateMode API provides the name precedent;
    // SkeltalAsset 125d49c consumes the original 0x7000 rotation-mode bits.
    nn::Bit32 GetRotateMode() const { return mFlags & 0x7000; }

private:
    // Partial resource layout; unidentified intervals retain original offsets.
    u8 _0[0x10];
    nn::util::BinPtrToString mName;
    u8 _18[8];
    // Reset ac720c clears this original pointer; its owner is unidentified.
    void* _20;
    // Reset ac720c fills one u16 per bone animation with the invalid index.
    u16* _28;
    ResBoneAnim* mBoneAnims;
    u8 _38[0x10];
    u32 mFlags;
    s32 mFrameCount;
    u8 _50[8];
    u16 mBoneAnimCount;
};
}  // namespace nn::g3d
