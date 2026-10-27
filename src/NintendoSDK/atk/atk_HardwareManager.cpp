#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

int HardwareManager::GetChannelCountMax() const {
    return _758 ? 2 : 6;
}

int HardwareManager::GetRequiredMemSizeForMemoryPool(int max_voices) const {
    return LowLevelVoiceAllocator::GetRequiredMemSize(max_voices);
}

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn

namespace nn {
namespace atk {
namespace detail {
namespace driver {

void HardwareManager::FlushDataCache(void*, size_t) {
}

void HardwareManager::LockEffectAuxList() {
    nn::os::LockMutex(&mEffectAuxMutex);
}

void HardwareManager::UnlockEffectAuxList() {
    nn::os::UnlockMutex(&mEffectAuxMutex);
}

bool HardwareManager::TimedWaitAudioRendererEvent(nn::TimeSpan span) {
    return nn::os::TimedWaitSystemEvent(&mAudioRendererEvent, span);
}

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn
