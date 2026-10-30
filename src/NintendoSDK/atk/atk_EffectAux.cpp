#include <nn/atk/EffectAux.h>
#include <nn/atk/detail/Util.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk {

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

bool EffectAux::IsRemovable() const {
    return nn::audio::IsAuxRemovable(&mAux);
}

}  // namespace nn::atk

