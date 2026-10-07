#include <nn/g3d/ModelAnimObj.h>
#include <nn/g3d/SkeletalAnimObj.h>
#include <nn/g3d/ResSkeleton.h>

namespace nn::g3d {

float AnimFrameCtrl::PlayLoop(float frame, float start, float end, void*) {
    float direction;
    float origin;
    if (frame >= end) {
        frame -= end;
        direction = 1.0f;
        origin = start;
    } else if (frame < start) {
        frame = start - frame;
        direction = -1.0f;
        origin = end;
    } else {
        return frame;
    }
    const float length = end - start;
    if (length == 0.0f)
        return start;
    return origin + direction * (frame - length * s32(frame / length));
}

void AnimBindTable::sub_710132B58C(u32* entries, s32 capacity) {
    mEntries = entries;
    mFlags = 0;
    mCapacity = capacity;
    mAnimCount = 0;
    mTargetCount = 0;
}

void AnimBindTable::BindAll(const u16* indices) {
    for (s32 i = 0; i < mAnimCount; ++i) {
        const u16 target = indices[i];
        if (target < 0x7fff) {
            mEntries[i] &= 0x3fff8000;
            mEntries[i] |= target & 0x7fff;
            mEntries[target] &= 0xc0007fff;
            mEntries[target] |= (u32(i) << 15) & 0x3fff8000;
        }
    }
}

void ModelAnimObj::SetBindFlagImpl(s32 index, BindFlag flag) {
    const u32 anim_index = (mBindTable.mEntries[index] >> 15) & 0x7fff;
    if (anim_index != 0x7fff) {
        mBindTable.mEntries[anim_index] &= 0x3fffffff;
        mBindTable.mEntries[anim_index] |= u32(flag) << 30;
    }
}

// NON_MATCHING: bind flag shifting and temporary registers differ.
void SkeletalAnimObj::sub_7101333EC0(const ResSkeleton* skeleton, s32 first_bone,
                                  BindFlag flag) {
    const s32 end = skeleton->GetBranchEndIndex(first_bone);
    // A valid branch contains its root bone; the original processes it before testing the end.
    s32 bone = first_bone;
    do {
        SetBindFlagImpl(bone, flag);
    } while (++bone < end);
}

}  // namespace nn::g3d
