#include <nn/g3d/ResSkeletalAnim.h>
#include <nn/g3d/ResAnimCurve.h>

namespace nn::g3d {
// NON_MATCHING: curve iteration uses different record addressing.
void ResSkeletalAnim::Reset() {
    _20 = nullptr;
    const int index_count = mBoneAnimCount;
    u16* indices = _28;
    for (int i = 0; i < index_count; ++i)
        indices[i] = 0xffff;
    if (mFlags & 1) {
        const int bone_count = mBoneAnimCount;
        for (int i = 0; i < bone_count; ++i) {
            auto& bone = mBoneAnims[i].ToData();
            const int curve_count = bone.curveCount;
            for (int j = 0; j < curve_count; ++j) {
                auto& curve = bone.pCurves.Get()[j];
                if ((curve.ToData().flag & 0x70) < 0x40)
                    curve.ResetFloat();
                else
                    curve.sub_7100AC82FC();
            }
        }
        mFlags ^= 1;
    }
}
}  // namespace nn::g3d
