#include <nn/atk/detail/SequenceSound.h>

namespace nn::atk::detail {

bool SequenceSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

void SequenceSound::DetachTempSpecialHandle() {
    mTempSpecialHandle->DetachSound();
}

SoundPlayerHandle* SequenceSound::GetBasicSoundPlayerHandle() {
    return &mBasicSoundPlayerHandle;
}

}  // namespace nn::atk::detail
