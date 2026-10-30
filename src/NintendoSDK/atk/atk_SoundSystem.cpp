#include <nn/atk/SoundSystem.h>
#include <nn/atk/detail/Util.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk {

// 0x7102662aa8: the flag IsInitialized returns and Finalize clears, both through the GOT.
bool sIsInitialized = false;

int SoundSystem::GetRendererChannelCountMax() {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().GetChannelCountMax();
}

bool SoundSystem::IsClearEffectFinished(AuxBus bus, OutputDevice device) {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().IsFinishedClearEffect(
        bus, device, 0);
}

// 0x710133e244
int SoundSystem::GetRendererSampleCount() {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
        .GetRendererSampleCount();
}

// 0x710133dd80
bool SoundSystem::IsInitialized() {
    return sIsInitialized;
}

void SoundSystem::AttachMemoryPool(nn::audio::MemoryPoolType* pool, void* address, size_t size) {
    detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().AttachMemoryPool(
        pool, address, size, true);
}

void SoundSystem::DetachMemoryPool(nn::audio::MemoryPoolType* pool) {
    detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().DetachMemoryPool(pool, true);
}

}  // namespace nn::atk
