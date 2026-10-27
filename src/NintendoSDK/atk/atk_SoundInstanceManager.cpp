#include <nn/atk/detail/AdvancedWaveSoundRuntime.h>
#include <nn/atk/detail/BasicSound.h>
#include <nn/atk/detail/SequenceSoundRuntime.h>
#include <nn/atk/detail/SoundInstanceManager.h>
#include <nn/atk/detail/WaveSoundRuntime.h>

#include <algorithm>

namespace nn {
namespace atk {
namespace detail {

void AdvancedWaveSoundRuntime::Update() {
    SortPriorityList();
}

void SequenceSoundRuntime::Update() {
    SortPriorityList();
}

void WaveSoundRuntime::Update() {
    SortPriorityList();
}

// Kept out of atk_BasicSound.cpp, where the two-argument overload has to stay an out-of-line
// tail call to this one.
// NON_MATCHING: the two fade paths reorder the fade-frame multiply and the max against the
// volume stores (the pause arm also picks a different temporary for the truncation).
void BasicSound::Pause(bool pause, s32 fadeFrames, PauseMode mode) {
    s32 time;
    u8 state;
    u8 resume;
    if (pause) {
        if (mPauseState != 0 && mPauseState != 1 && mPauseState != 3)
            return;

        const f32 frames = static_cast<f32>(fadeFrames);
        f32 fadeVolume = mPauseFadeVolume;
        if (mPauseFadeFrame < mPauseFadeTime)
            fadeVolume = mPauseFadePrevVolume + (fadeVolume - mPauseFadePrevVolume) *
                                                    static_cast<f32>(mPauseFadeFrame) /
                                                    static_cast<f32>(mPauseFadeTime);
        time = std::max(s32(frames * fadeVolume), 1);
        mPauseFadePrevVolume = fadeVolume;
        mPauseFadeVolume = 0.0f;
        state = 1;
        resume = 0;
    } else {
        if (u32(mPauseState - 1) > 2)
            return;

        const f32 frames = static_cast<f32>(fadeFrames);
        f32 fadeVolume = mPauseFadeVolume;
        if (mPauseFadeFrame < mPauseFadeTime)
            fadeVolume = mPauseFadePrevVolume + (fadeVolume - mPauseFadePrevVolume) *
                                                    static_cast<f32>(mPauseFadeFrame) /
                                                    static_cast<f32>(mPauseFadeTime);
        time = std::max(s32(frames * (1.0f - fadeVolume)), 1);
        mPauseFadePrevVolume = fadeVolume;
        mPauseFadeVolume = 1.0f;
        state = 3;
        resume = 1;
    }
    mPauseFadeTime = time;
    mPauseFadeFrame = 0;
    mPauseState = state;
    mPauseResume = resume;
    mPauseMode = mode;
}

AdvancedWaveSoundRuntime::~AdvancedWaveSoundRuntime() {
}

SequenceSoundRuntime::~SequenceSoundRuntime() {
    mArchiveManager = nullptr;
}

WaveSoundRuntime::~WaveSoundRuntime() {
}

}  // namespace detail
}  // namespace atk
}  // namespace nn

