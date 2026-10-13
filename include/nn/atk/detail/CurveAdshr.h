#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Complete value owner: Channel construction places this 0x1c-byte envelope
// before its first LFO at +0x3c. Field names describe the full native operations.
class CurveAdshr {
public:
    CurveAdshr();
    void Initialize(f32 value);
    void Reset(f32 value);
    f32 GetValue() const;
    void SetDecay(int decay);
    void SetRelease(int release);

private:
    u32 mState;
    f32 mValue;
    f32 mDecayRate;
    f32 mReleaseRate;
    f32 mAttackRate;
    u16 mHoldTime;
    // Native Update fills the remaining count when attack enters hold.
    u16 mRemainingHoldTime;
    u8 mSustain;
};
static_assert(sizeof(CurveAdshr) == 0x1c);

}  // namespace nn::atk::detail
