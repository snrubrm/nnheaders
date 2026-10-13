#include <nn/atk/detail/CurveAdshr.h>

namespace nn::atk::detail {

// NON_MATCHING: constant stores are grouped differently.
CurveAdshr::CurveAdshr() {
    Initialize(-90.4f);
}

// NON_MATCHING: default-control stores are grouped differently.
void CurveAdshr::Initialize(f32 value) {
    mDecayRate = 65535.0f;
    mReleaseRate = 65535.0f;
    mAttackRate = 0.0f;
    mHoldTime = 0;
    mSustain = 127;
    Reset(value);
}

void CurveAdshr::Reset(f32 value) {
    mValue = value * 10.0f;
    mState = 0;
}

f32 CurveAdshr::GetValue() const {
    if (mState == 0 && mAttackRate == 0.0f)
        return 0.0f;
    return mValue / 10.0f;
}

// NON_MATCHING: the two special-case blocks are ordered differently.
void CurveAdshr::SetDecay(int decay) {
    switch (decay) {
    case 127:
        mDecayRate = 65535.0f;
        return;
    case 126:
        mDecayRate = 24.0f;
        return;
    default:
        break;
    }
    f32 rate;
    if (decay < 50)
        rate = f32(decay * 2 + 1) / 128.0f;
    else
        rate = 60.0f / f32(126 - decay);
    mDecayRate = rate / 5.0f;
}

// NON_MATCHING: the two special-case blocks are ordered differently.
void CurveAdshr::SetRelease(int release) {
    switch (release) {
    case 127:
        mReleaseRate = 65535.0f;
        return;
    case 126:
        mReleaseRate = 24.0f;
        return;
    default:
        break;
    }
    f32 rate;
    if (release < 50)
        rate = f32(release * 2 + 1) / 128.0f;
    else
        rate = 60.0f / f32(126 - release);
    mReleaseRate = rate / 5.0f;
}

}  // namespace nn::atk::detail
