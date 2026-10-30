#include <nn/atk/detail/driver/HardwareManager.h>
#include <nn/atk/EffectAux.h>

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

size_t HardwareManager::GetRequiredEffectAuxBufferSize(const EffectAux* effect) const {
    return effect->GetRequiredMemSize(mRendererParameter);
}

// NON_MATCHING: the compiler places the renderer parameter at a different stack offset.
size_t HardwareManager::GetRequiredPerformanceFramesBufferSize(
    const HardwareManagerParameter& parameter) const {
    nn::audio::AudioRendererParameter renderer_parameter;
    SetupAudioRendererParameter(&renderer_parameter, parameter);
    const size_t size = nn::audio::GetRequiredBufferSizeForPerformanceFrames(renderer_parameter);
    return (size + 63) & ~63ULL;
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
