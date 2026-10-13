#include <nn/atk/SoundArchivePlayer.h>

namespace nn::atk {

bool SoundArchivePlayer::IsAvailable() const {
    return mArchiveManager.IsAvailable();
}

const SoundArchive* SoundArchivePlayer::GetSoundArchive() const {
    return mArchiveManager.mSoundArchive;
}

}  // namespace nn::atk
