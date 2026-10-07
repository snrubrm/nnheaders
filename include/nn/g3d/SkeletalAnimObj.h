#pragma once

#include <nn/g3d/ModelAnimObj.h>

namespace nn::g3d {

class ResBoneAnim;
class ResSkeletalAnim;
class ResSkeleton;

// RTTI at 0x710252cc40 identifies ModelAnimObj as the direct base.
// Initialize/SetResource (0x71013331dc/0x71013332e0) establish the fields.
class SkeletalAnimObj : public ModelAnimObj {
public:
    ~SkeletalAnimObj() override;
    void ClearResult() override;
    void Calculate() override;

    void sub_7101333EC0(const ResSkeleton* skeleton, s32 first_bone, BindFlag flag);

private:
    const ResSkeletalAnim* mResource;
    const ResBoneAnim* mBoneAnims;
    s32 mMaxBoneAnimCount;
    u32 mFlags;
    void* _80;
};

static_assert(sizeof(SkeletalAnimObj) == 0x88);

}  // namespace nn::g3d
