#include <nn/atk/detail/driver/SoundThread.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk::detail::driver {

void SoundThread::RegisterPlayerCallback(PlayerCallback* callback) {
    mPlayerCallbacks.push_back(callback->mNode);
}

void SoundThread::UnregisterPlayerCallback(PlayerCallback* callback) {
    mPlayerCallbacks.erase(mPlayerCallbacks.iterator_to(callback->mNode));
}

void SoundThread::ForceWakeup() {
}

void SoundThread::EffectFrameProcess() {
    Util::Singleton<HardwareManager>::GetInstance().UpdateEffect();
}

}  // namespace nn::atk::detail::driver
