#include <nn/atk/SoundSystem.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk {

int SoundSystem::GetRendererChannelCountMax() {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().GetChannelCountMax();
}

}  // namespace nn::atk
