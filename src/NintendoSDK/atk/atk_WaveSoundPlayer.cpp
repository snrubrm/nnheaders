#include <nn/atk/detail/driver/WaveSoundPlayer.h>

namespace nn::atk::detail::driver {

void WaveSoundPlayer::SetChannelPriority(int priority) {
    mChannelPriority = priority;
}

void WaveSoundPlayer::SetReleasePriorityFix(bool fix) {
    mReleasePriorityFix = fix;
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
