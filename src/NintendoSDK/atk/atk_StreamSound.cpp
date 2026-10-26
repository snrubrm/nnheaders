#include <nn/atk/detail/StreamSound.h>

namespace nn::atk::detail {

bool StreamSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

void StreamSound::DetachTempSpecialHandle() {
    mTempSpecialHandle->DetachSound();
}

SoundPlayerHandle* StreamSound::GetBasicSoundPlayerHandle() {
    return &mBasicSoundPlayerHandle;
}

}  // namespace nn::atk::detail
