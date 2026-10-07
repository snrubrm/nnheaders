#pragma once

#include <nn/types.h>
#include <nn/util/AccessorBase.h>
#include <nn/util/util_BinTypes.h>

namespace nn::g3d {
class AnimFrameCache;

// Reset ac720c walks 0x30-byte records. EvaluateFloat ac7fd8 and the
// typed baked evaluators independently establish the flags, key array and frames.
struct ResAnimCurveData {
    nn::util::BinTPtr<void> pFrames;
    nn::util::BinTPtr<void> pKeys;
    u16 flag;
    u16 keyCount;
    u32 targetOffset;
    float startFrame;
    float endFrame;
    float scale;
    float offset;
    float delta;
    u32 _2c;
};

class ResAnimCurve : public nn::util::AccessorBase<ResAnimCurveData> {
public:
    void ResetFloat();
    void sub_7100AC82FC();
    template <typename T>
    int EvaluateBakedInt(float frame, AnimFrameCache* cache) const;
    template <typename T>
    float EvaluateBakedFloat(float frame, AnimFrameCache* cache) const;
    int EvaluateBakedBool(float frame, AnimFrameCache* cache) const;
};
static_assert(sizeof(ResAnimCurve) == 0x30);
}  // namespace nn::g3d
