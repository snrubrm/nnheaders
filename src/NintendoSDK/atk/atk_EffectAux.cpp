#include <nn/atk/EffectAux.h>
#include <nn/atk/detail/Util.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk {

EffectAux::EffectAux()
    : mAudioFrameCount(4), mChannelCount(4), mIsAdded(false), mIsEnabled(false),
      mBuffer(nullptr), mBufferSize(0), mSampleBuffer(nullptr), mChannelIndices{0, 1, 2, 3, 4, 5} {}

EffectAux::~EffectAux() {}

bool EffectAux::Initialize() {
    return true;
}

void EffectAux::Finalize() {}

void EffectAux::OnChangeOutputMode() {}

void EffectAux::SetEffectBuffer(void* buffer, u64 size) {
    mBuffer = buffer;
    mBufferSize = size;
}

size_t EffectAux::GetRequiredMemSize(const nn::audio::AudioRendererParameter& parameter) const {
    return nn::audio::GetRequiredBufferSizeForAuxSendReturnBuffer(&parameter, mAudioFrameCount,
                                                                  mChannelCount) * 3;
}

bool EffectAux::SetChannelCount(s32 count) {
    for (u32 i = 0; i < 6; ++i)
        mChannelIndices[i] = i;
    if (mIsAdded)
        return false;
    mChannelCount = count;
    return true;
}

bool EffectAux::SetAudioFrameCount(s32 count) {
    if (mIsAdded)
        return false;
    mAudioFrameCount = count;
    return true;
}

void EffectAux::SetEnabled(bool enabled) {
    mIsEnabled = enabled;
    if (mIsAdded) {
        nn::os::LockMutex(detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                              .GetAudioRendererMutex());
        nn::audio::SetAuxEnabled(&mAux, enabled);
        nn::os::UnlockMutex(detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                                .GetAudioRendererMutex());
    }
}

void EffectAux::SetEffectInputOutput(const s8* input, const s8*, int, int) {
    s8 channels[6];
    for (s32 i = 0; i < mChannelCount; ++i)
        channels[i] = input[mChannelIndices[i]];
    nn::audio::SetAuxInputOutput(&mAux, channels, channels, mChannelCount);
}

void EffectAux::RemoveEffect(nn::audio::AudioRendererConfig* config, nn::audio::FinalMixType* mix) {
    if (mIsAdded) {
        nn::os::LockMutex(detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                              .GetAudioRendererMutex());
        nn::audio::RemoveAux(config, &mAux, mix);
        mIsAdded = false;
        nn::os::UnlockMutex(detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                                .GetAudioRendererMutex());
    }
}

void EffectAux::RemoveEffect(nn::audio::AudioRendererConfig* config, nn::audio::SubMixType* mix) {
    if (mIsAdded) {
        nn::os::LockMutex(detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                              .GetAudioRendererMutex());
        nn::audio::RemoveAux(config, &mAux, mix);
        mIsAdded = false;
        nn::os::UnlockMutex(detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                                .GetAudioRendererMutex());
    }
}

void EffectAux::Update() {
    if (!mIsAdded)
        return;

    const s32 sampleCount = nn::audio::GetAuxSampleCount(&mAux);
    UpdateSamplesArg arg;
    arg.samplesPerFrame = sampleCount / (mAudioFrameCount * mChannelCount);
    arg.sampleRate = nn::audio::GetAuxSampleRate(&mAux);
    arg.audioFrameCount = mAudioFrameCount;
    arg.channelCount = mChannelCount;
    arg.outputMode = detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
                         .GetOutputMode();
    const s32 readCount = nn::audio::ReadAuxSendBuffer(&mAux, mSampleBuffer, sampleCount);
    arg.sampleCount = readCount;
    UpdateSamples(mSampleBuffer, arg);
    nn::audio::WriteAuxReturnBuffer(&mAux, mSampleBuffer, readCount);
}

bool EffectAux::IsRemovable() const {
    return nn::audio::IsAuxRemovable(&mAux);
}

}  // namespace nn::atk

