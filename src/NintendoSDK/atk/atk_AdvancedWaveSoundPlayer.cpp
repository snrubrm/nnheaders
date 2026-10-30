#include <nn/atk/detail/driver/AdvancedWaveSoundPlayer.h>

namespace nn::atk::detail::driver {

void AdvancedWaveSoundPlayer::Initialize() {
    if (_765 == 0) {
        BasicSoundPlayer::Initialize();
        _765 = 1;
    }
    _764 = 0;
}

void AdvancedWaveSoundPlayer::Finalize() {
    SetFinished(true);
    if (_766) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        _766 = 0;
    }
    if (IsStarted())
        SetStarted(false);
    if (IsPrepared())
        SetPrepared(false);
    if (_765) {
        BasicSoundPlayer::Finalize();
        _765 = 0;
    }
}

void AdvancedWaveSoundPlayer::Start() {
    SetStarted(true);
}

void AdvancedWaveSoundPlayer::Stop() {
    SetFinished(true);
    if (_766) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        _766 = 0;
    }
    if (IsStarted())
        SetStarted(false);
}

void AdvancedWaveSoundPlayer::Pause(bool pause) {
    SetPaused(pause);
}

// NON_MATCHING: the secondary-base thunk retains its incoming callback pointer
// instead of materializing the complete player pointer early.
void AdvancedWaveSoundPlayer::OnUpdateFrameSoundThread() {
    if (IsPrepared() && IsStarted() && !IsPaused() && UpdateTracks()) {
        SetFinished(true);
        if (_766) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
            _766 = 0;
        }
        if (IsStarted())
            SetStarted(false);
    }
}

// NON_MATCHING: the secondary-base thunk retains its incoming callback pointer
// instead of materializing the complete player pointer early.
void AdvancedWaveSoundPlayer::OnUpdateFrameSoundThreadWithAudioFrameFrequency() {
    if (_758 == 0 && IsPrepared() && IsStarted() && !IsPaused() && UpdateTracks()) {
        SetFinished(true);
        if (_766) {
            SoundThread::GetInstance().UnregisterPlayerCallback(this);
            _766 = 0;
        }
        if (IsStarted())
            SetStarted(false);
    }
}

void AdvancedWaveSoundPlayer::OnShutdownSoundThread() {
    Stop();
}

}  // namespace nn::atk::detail::driver
