#include <nn/g3d/ResAnimCurve.h>

namespace nn::g3d {
template <typename T>
int ResAnimCurve::EvaluateBakedInt(float frame, AnimFrameCache*) const {
    const int start = int(startFrame);
    const int index = int(frame) - start;
    return static_cast<const T*>(pKeys.Get())[index];
}
template <typename T>
float ResAnimCurve::EvaluateBakedFloat(float frame, AnimFrameCache*) const {
    const int start = int(startFrame);
    const int frame_int = int(frame);
    const int index = frame_int - start;
    const float weight = frame - frame_int;
    const T* keys = static_cast<const T*>(pKeys.Get());
    const float next = keys[index + 1];
    const float prev = keys[index];
    return weight * next + (1.0f - weight) * prev;
}
int ResAnimCurve::EvaluateBakedBool(float frame, AnimFrameCache*) const {
    const int start = int(startFrame);
    const int index = int(frame) - start;
    return (static_cast<const u32*>(pKeys.Get())[index >> 5] >> (index & 31)) & 1;
}
template int ResAnimCurve::EvaluateBakedInt<s32>(float, AnimFrameCache*) const;
template int ResAnimCurve::EvaluateBakedInt<s16>(float, AnimFrameCache*) const;
template int ResAnimCurve::EvaluateBakedInt<s8>(float, AnimFrameCache*) const;
template float ResAnimCurve::EvaluateBakedFloat<float>(float, AnimFrameCache*) const;
template float ResAnimCurve::EvaluateBakedFloat<s16>(float, AnimFrameCache*) const;
template float ResAnimCurve::EvaluateBakedFloat<s8>(float, AnimFrameCache*) const;
}  // namespace nn::g3d
