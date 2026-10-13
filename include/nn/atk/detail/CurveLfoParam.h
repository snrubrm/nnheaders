#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Complete 0x10-byte parameter value: independent native producers initialize
// arrays at that stride. Member names describe the full Update/GetValue uses.
struct CurveLfoParam {
    void Initialize();

    f32 mDepth;
    f32 mSpeed;
    u32 mDelay;
    u8 mRange;
    u8 mCurve;
    u8 mPhase;
};
static_assert(sizeof(CurveLfoParam) == 0x10);
static_assert(offsetof(CurveLfoParam, mPhase) == 0xe);

}  // namespace nn::atk::detail
