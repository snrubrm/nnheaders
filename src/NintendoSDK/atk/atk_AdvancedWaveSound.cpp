#include <nn/atk/detail/AdvancedWaveSound.h>

namespace nn::atk::detail {

bool AdvancedWaveSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

void AdvancedWaveSound::DetachTempSpecialHandle() {
}

SoundPlayerHandle* AdvancedWaveSound::GetBasicSoundPlayerHandle() {
    return &mBasicSoundPlayerHandle;
}

}  // namespace nn::atk::detail
