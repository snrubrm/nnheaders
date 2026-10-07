/**
 * @file ResSkeletalAnim.h
 * @brief Resource file for skeletal animations.
 */

#pragma once

#include <nn/types.h>

namespace nn::g3d {
class ResSkeletalAnim {
public:
    void Reset();

    // Inline-only in the original; names are reconstruction guesses. The signed
    // frame count at +0x4c and loop bit2 at +0x48 are read by both SkeltalAsset's
    // constructor (0x710125c4b4/0x710125c4fc) and SDK SetResource (0x710133330c).
    s32 GetFrameCount() const { return mFrameCount; }
    bool IsLooped() const { return (mFlags & 4) != 0; }

private:
    // Partial resource layout through the frame count; the remaining members are unmodeled.
    u8 _0[0x48];
    u32 mFlags;
    s32 mFrameCount;
};
}  // namespace nn::g3d
