#include <nn/atk/detail/SequenceSound.h>

namespace nn::atk::detail {

bool SequenceSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

SoundPlayerHandle* SequenceSound::GetBasicSoundPlayerHandle() {
    return &mBasicSoundPlayerHandle;
}

}  // namespace nn::atk::detail
