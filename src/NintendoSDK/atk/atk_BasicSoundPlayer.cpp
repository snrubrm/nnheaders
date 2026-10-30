#include <nn/atk/detail/driver/BasicSoundPlayer.h>

namespace nn::atk::detail::driver {

void BasicSoundPlayer::Finalize() {
    mEvent.Signal();
}

void BasicSoundPlayer::SetBiquadFilter(int type, float frequency) {
    mBiquadType = type;
    mBiquadFrequency = frequency;
}

}  // namespace nn::atk::detail::driver
