#include <nn/atk/detail/driver/WaveSoundPlayer.h>

namespace nn::atk::detail::driver {

void WaveSoundPlayer::Start() {
    SetStarted(true);
}

void WaveSoundPlayer::Pause(bool pause) {
    SetPaused(pause);
    if (mChannel)
        mChannel->Pause(pause);
}

void WaveSoundPlayer::FinishPlayer() {
    SetFinished(true);
    if (_14a) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        _14a = 0;
    }
    if (IsStarted())
        SetStarted(false);
}

void WaveSoundPlayer::SetChannelPriority(int priority) {
    mChannelPriority = priority;
}

void WaveSoundPlayer::SetReleasePriorityFix(bool fix) {
    mReleasePriorityFix = fix;
}

s64 WaveSoundPlayer::GetPlaySamplePosition(bool flag) const {
    return mChannel ? mChannel->GetCurrentPlayingSample(flag) : 0;
}

void WaveSoundPlayer::ChannelCallbackFunc(Channel*, Channel::ChannelCallbackStatus, void* arg) {
    static_cast<WaveSoundPlayer*>(arg)->mChannel = nullptr;
}

void WaveSoundPlayer::OnUpdateFrameSoundThread() {
    Update();
}

void WaveSoundPlayer::OnUpdateFrameSoundThreadWithAudioFrameFrequency() {
    if (_140 == 0)
        Update();
}

void WaveSoundPlayer::OnShutdownSoundThread() {
    Stop();
}

}  // namespace nn::atk::detail::driver
