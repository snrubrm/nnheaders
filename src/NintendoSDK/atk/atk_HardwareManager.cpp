#include <nn/atk/detail/driver/HardwareManager.h>
#include <nn/atk/EffectAux.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

void HardwareManager::SetupAudioRendererParameter(
    nn::audio::AudioRendererParameter* renderer, const HardwareManagerParameter& parameter) const {
    const u32 channel_count = parameter._1a ? 2 : 6;
    nn::audio::InitializeAudioRendererParameter(renderer);
    renderer->sampleRate = parameter.mSampleRate;
    renderer->sampleCount = parameter.mSampleRate * 5 / 1000;
    renderer->mixBufferCount = 30;
    renderer->subMixCount = 1;
    renderer->voiceCount = parameter.mVoiceCount;
    renderer->sinkCount = 1;
    renderer->effectCount = parameter.mEffectCount + 4;
    renderer->performanceFrameCount = 0;
    renderer->isVoiceDropEnabled = parameter.mVoiceDropEnabled;
    if (parameter._8)
        renderer->performanceFrameCount = 15;

    u32 mix_buffer_count = 30;
    if (!parameter._12) {
        mix_buffer_count = channel_count * 2;
        renderer->mixBufferCount = mix_buffer_count;
        renderer->effectCount = 0;
    }
    u32 sink_count = 2;
    if (parameter._13) {
        renderer->sinkCount = 2;
        sink_count = 3;
    }
    if (parameter._18)
        renderer->sinkCount = sink_count;

    u32 sub_mix_count = 2;
    if (parameter._10) {
        mix_buffer_count += parameter._12 ? 6 : 2;
        renderer->mixBufferCount = mix_buffer_count;
        renderer->subMixCount = 2;
        sub_mix_count = 3;
    }
    if (parameter._11) {
        mix_buffer_count += parameter._12 ? channel_count : 0;
        renderer->mixBufferCount = mix_buffer_count;
        renderer->subMixCount = sub_mix_count;
    }
    if (!parameter._19) {
        renderer->mixBufferCount = channel_count;
        renderer->subMixCount = 0;
    }
}

int HardwareManager::GetChannelCountMax() const {
    return _758 ? 2 : 6;
}

size_t HardwareManager::GetRequiredMemSizeForMemoryPool(int max_voices) const {
    return LowLevelVoiceAllocator::GetRequiredMemSize(max_voices);
}

// NON_MATCHING: the compiler places the renderer parameter at a different stack offset.
size_t HardwareManager::GetRequiredMemSize(const HardwareManagerParameter& parameter) const {
    nn::audio::AudioRendererParameter renderer_parameter;
    SetupAudioRendererParameter(&renderer_parameter, parameter);
    const size_t renderer_size = nn::audio::GetAudioRendererWorkBufferSize(renderer_parameter);
    const size_t config_size = nn::audio::GetAudioRendererConfigWorkBufferSize(renderer_parameter);
    return renderer_size + config_size;
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

// NON_MATCHING: the compiler places the renderer parameter at a different stack offset.
size_t HardwareManager::GetRequiredCircularBufferSinkWithMemoryPoolBufferSize(
    const HardwareManagerParameter& parameter) const {
    nn::audio::AudioRendererParameter renderer_parameter;
    SetupAudioRendererParameter(&renderer_parameter, parameter);
    const size_t size = nn::audio::GetRequiredBufferSizeForCircularBufferSink(
        &renderer_parameter, _758 ? 2 : 6, parameter.mRecorderFrameCount,
        nn::audio::SampleFormat_PcmInt16);
    return (size + 0x1fff) & ~0xfffULL;
}

// NON_MATCHING: the compiler places the renderer parameter at a different stack offset.
size_t HardwareManager::GetRequiredRecorderWorkBufferSize(
    const HardwareManagerParameter& parameter) const {
    nn::audio::AudioRendererParameter renderer_parameter;
    SetupAudioRendererParameter(&renderer_parameter, parameter);
    const size_t sink_size = nn::audio::GetRequiredBufferSizeForCircularBufferSink(
        &renderer_parameter, _758 ? 2 : 6, parameter.mRecorderFrameCount,
        nn::audio::SampleFormat_PcmInt16);
    const size_t pool_size = (sink_size + 0x1fff) & ~0xfffULL;
    SetupAudioRendererParameter(&renderer_parameter, parameter);
    const size_t work_size = nn::audio::GetRequiredBufferSizeForCircularBufferSink(
        &renderer_parameter, _758 ? 2 : 6, parameter.mRecorderFrameCount,
        nn::audio::SampleFormat_PcmInt16);
    return pool_size + work_size;
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
