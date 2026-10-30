#pragma once

#include <nn/atk/detail/CurveLfoParam.h>

namespace nn::atk::detail {

class CurveLfo {
public:
    static void InitializeCurveTable();
    void Reset();
    void Update(int time);
    f32 GetValue() const;

private:
    CurveLfoParam mParam;
    u32 mElapsed;
    f32 mPhase;
    mutable f32 mCachedValue;
    bool mPhaseInitialized;
    bool mPhaseWrapped;
};
static_assert(sizeof(CurveLfo) == 0x20);

}  // namespace nn::atk::detail
