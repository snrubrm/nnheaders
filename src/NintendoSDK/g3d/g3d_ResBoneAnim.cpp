#include <nn/g3d/ResBoneAnim.h>
#include <nn/g3d/ResSkeleton.h>
#include <cstring>

namespace nn::g3d {
// NON_MATCHING: translation selection is folded and stores are scheduled differently.
void ResBoneAnim::sub_7100AC6F68(BoneAnimResult* result, const ResBone* bone) const {
    const u32 flags = flag;
    const float* base = pBaseValues.Get();
    // Original constants at 1e7f744 and 1e79518 are scale-one and identity rotation.
    static const nn::util::Float3 scale_one = {{1, 1, 1}};
    static const nn::util::Float4 identity_rotation = {{0, 0, 0, 1}};
    if (flags & 8) {
        std::memcpy(&result->scale, base, sizeof(result->scale));
        base += 3;
    } else {
        result->scale = bone ? bone->GetScale() : scale_one;
    }
    if (flags & 16) {
        std::memcpy(&result->rotate, base, sizeof(result->rotate));
        base += 4;
    } else {
        result->rotate = bone ? bone->GetRotateQuat() : identity_rotation;
    }
    if (!(flags & 32))
        base = bone ? bone->GetTranslate().v : identity_rotation.v;
    std::memcpy(&result->translate, base, sizeof(result->translate));
    result->flag = flags & 0x0f800000;
}
}  // namespace nn::g3d
