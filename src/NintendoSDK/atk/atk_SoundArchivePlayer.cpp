#include <nn/atk/SoundArchivePlayer.h>
#include <nn/atk/detail/StreamSoundRuntime.h>

namespace nn::atk {

bool SoundArchivePlayer::IsAvailable() const {
    return mArchiveManager.IsAvailable();
}

const SoundArchive* SoundArchivePlayer::GetSoundArchive() const {
    return mArchiveManager.mSoundArchive;
}

size_t SoundArchivePlayer::GetRequiredMemSize(const SoundArchive* archive) {
    return GetRequiredMemSize(archive, 0, 0);
}

size_t SoundArchivePlayer::GetRequiredMemSize(const SoundArchive* archive, size_t unknown) {
    return GetRequiredMemSize(archive, unknown, 0);
}

size_t SoundArchivePlayer::GetRequiredStreamCacheSize(const SoundArchive* archive, size_t unknown) {
    return detail::StreamSoundRuntime::GetRequiredStreamCacheSize(archive, unknown);
}

}  // namespace nn::atk
