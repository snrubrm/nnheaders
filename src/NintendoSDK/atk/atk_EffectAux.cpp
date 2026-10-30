#include <nn/atk/EffectAux.h>

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

bool EffectAux::IsRemovable() const {
    return nn::audio::IsAuxRemovable(&mAux);
}

}  // namespace nn::atk

