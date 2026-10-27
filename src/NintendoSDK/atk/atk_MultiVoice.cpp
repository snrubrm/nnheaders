#include <nn/atk/detail/driver/MultiVoice.h>

#include <algorithm>

namespace nn::atk::detail::driver {

SampleFormat MultiVoice::GetFormat() const {
    return mFormat;
}

s64 MultiVoice::GetCurrentPlayingSample() const {
    if (mVoiceCount < 1)
        return 0;
    return GetPlayPosition();
}

// NON_MATCHING: the two independent stores are scheduled in the other order; the original puts
// mPaused before the flags store.
void MultiVoice::Start() {
    mStarted = true;
    mFlags |= 1;
    mPaused = false;
}

void MultiVoice::Pause(bool pause) {
    if (mPaused == pause)
        return;
    mPaused = pause;
    mFlags |= 2;
}

void MultiVoice::SetVolume(f32 volume) {
    const f32 value = std::max(volume, 0.0f);
    if (value == mVolume)
        return;
    mVolume = value;
    mFlags |= 0x40;
}

void MultiVoice::SetPitch(f32 pitch) {
    if (pitch == mPitch)
        return;
    mPitch = pitch;
    mFlags |= 4;
}

void MultiVoice::SetPanMode(nn::atk::PanMode mode) {
    if (mode == mPanMode)
        return;
    mPanMode = mode;
    mFlags |= 8;
}

void MultiVoice::SetPanCurve(nn::atk::PanCurve curve) {
    if (curve == mPanCurve)
        return;
    mPanCurve = curve;
    mFlags |= 8;
}

void MultiVoice::SetLpfFreq(f32 frequency) {
    if (frequency == mLpfFreq)
        return;
    mLpfFreq = frequency;
    mFlags |= 0x10;
}

void MultiVoice::SetOutputLine(u32 line) {
    if (line == mOutputLine)
        return;
    mOutputLine = line;
    mFlags |= 8;
}

}  // namespace nn::atk::detail::driver
