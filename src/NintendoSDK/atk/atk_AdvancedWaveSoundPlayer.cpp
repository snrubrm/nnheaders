#include <nn/atk/detail/driver/AdvancedWaveSoundPlayer.h>

namespace nn::atk::detail::driver {

void AdvancedWaveSoundPlayer::Initialize() {
    if (_765 == 0) {
        BasicSoundPlayer::Initialize();
        _765 = 1;
    }
    _764 = 0;
}

void AdvancedWaveSoundPlayer::Start() {
    SetStarted(true);
}

void AdvancedWaveSoundPlayer::Pause(bool pause) {
    SetPaused(pause);
}

void AdvancedWaveSoundPlayer::OnShutdownSoundThread() {
    Stop();
}

}  // namespace nn::atk::detail::driver
