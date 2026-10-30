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
    // Inline-only, name guessed: Channel::Release/NoteOff and SequenceTrack::NoteOn all
    // independently compare the envelope state at +0.
    u32 GetState() const { return mState; }
    void SetDecay(int decay);
    void SetRelease(int release);
    void SetAttack(int attack);
    // Native 135396c/13539f8 and independent sequence/wave consumers establish
    // the hold-time conversion and sustain byte. Names follow SetDecay/Release.
    void SetHold(int hold);
    void SetSustain(int sustain);
    void Update(int milliseconds);

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
