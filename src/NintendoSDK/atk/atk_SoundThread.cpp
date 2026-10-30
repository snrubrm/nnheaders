#include <nn/atk/detail/driver/SoundThread.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk::detail::driver {

void SoundThread::ForceWakeup() {
}

void SoundThread::EffectFrameProcess() {
    Util::Singleton<HardwareManager>::GetInstance().UpdateEffect();
}

}  // namespace nn::atk::detail::driver
