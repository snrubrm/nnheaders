#include <nn/atk/detail/driver/StreamSoundPlayer.h>

namespace nn::atk::detail::driver {

void StreamSoundPlayer::OnUpdateFrameSoundThreadWithAudioFrameFrequency() {
    if (_11040 == 0)
        Update();
}

void StreamSoundPlayer::OnShutdownSoundThread() {
    Stop();
}

// NON_MATCHING: the compiler folds the count load and sign extension together.
void StreamSoundPlayer::SetTrackVolume(u32 track_mask, f32 volume) {
    if (!track_mask || mTrackCount <= 0)
        return;
    int i = 0;
    do {
        if (track_mask & 1)
            mTracks[i].mVolume = volume;
        track_mask >>= 1;
        ++i;
    } while (track_mask && i < mTrackCount);
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackInitialVolume(u32 track_mask, u32 volume) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mInitialVolume = volume;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackOutputLine(u32 track_mask, u32 line) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mOutputLine = line;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::ResetTrackOutputLine(u32 track_mask) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mOutputLine = 0xffffffff;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackTvVolume(u32 track_mask, f32 volume) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mTvVolume = volume;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackTvPan(u32 track_mask, f32 pan) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mTvPan = pan;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackTvSurroundPan(u32 track_mask, f32 pan) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mTvSurroundPan = pan;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackTvMainSend(u32 track_mask, f32 send) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mTvMainSend = send;
}

// NON_MATCHING: the compiler reshapes the count and mask loop.
void StreamSoundPlayer::SetTrackTvFxSend(u32 track_mask, nn::atk::AuxBus bus, f32 send) {
    for (int i = 0; track_mask && i < mTrackCount; ++i, track_mask >>= 1)
        if (track_mask & 1)
            mTracks[i].mTvFxSend[bus] = send;
}

}  // namespace nn::atk::detail::driver
