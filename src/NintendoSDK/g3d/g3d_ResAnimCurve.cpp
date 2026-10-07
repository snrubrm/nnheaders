#include <nn/g3d/ResAnimCurve.h>

namespace nn::g3d {
// NON_MATCHING: frame conversion and key loads are scheduled differently.
template <typename T>
int ResAnimCurve::EvaluateBakedInt(float frame, AnimFrameCache*) const {
    return static_cast<const T*>(pKeys.Get())[int(frame) - int(startFrame)];
}
// NON_MATCHING: interpolation inputs and frame conversion are scheduled differently.
template <typename T>
float ResAnimCurve::EvaluateBakedFloat(float frame, AnimFrameCache*) const {
    const T* keys = static_cast<const T*>(pKeys.Get());
    const int index = int(frame) - int(startFrame);
    const float weight = frame - int(frame);
    return weight * keys[index + 1] + (1.0f - weight) * keys[index];
}
// NON_MATCHING: frame conversion is scheduled differently.
int ResAnimCurve::EvaluateBakedBool(float frame, AnimFrameCache*) const {
    const int index = int(frame) - int(startFrame);
    return (static_cast<const u32*>(pKeys.Get())[index >> 5] >> (index & 31)) & 1;
}
template int ResAnimCurve::EvaluateBakedInt<s32>(float, AnimFrameCache*) const;
template int ResAnimCurve::EvaluateBakedInt<s16>(float, AnimFrameCache*) const;
template int ResAnimCurve::EvaluateBakedInt<s8>(float, AnimFrameCache*) const;
template float ResAnimCurve::EvaluateBakedFloat<float>(float, AnimFrameCache*) const;
template float ResAnimCurve::EvaluateBakedFloat<s16>(float, AnimFrameCache*) const;
template float ResAnimCurve::EvaluateBakedFloat<s8>(float, AnimFrameCache*) const;
}  // namespace nn::g3d
