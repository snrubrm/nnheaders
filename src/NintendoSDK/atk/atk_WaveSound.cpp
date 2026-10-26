#include <nn/atk/detail/WaveSound.h>

namespace nn::atk::detail {

bool WaveSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

SoundPlayerHandle* WaveSound::GetBasicSoundPlayerHandle() {
    return &mBasicSoundPlayerHandle;
}

}  // namespace nn::atk::detail
