#include <nn/atk/SoundSystem.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk {

int SoundSystem::GetRendererChannelCountMax() {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().GetChannelCountMax();
}

bool SoundSystem::IsClearEffectFinished(AuxBus bus, OutputDevice device) {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().IsFinishedClearEffect(
        bus, device, 0);
}

}  // namespace nn::atk
