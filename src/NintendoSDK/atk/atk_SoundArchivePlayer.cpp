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

Result SoundArchivePlayer::detail_SetupSound(SoundHandle* handle, u32 sound_id, bool hold,
                                            const char* label, const StartInfo* start_info) {
    return sub_710133B488(handle, sound_id, nullptr, nullptr, hold, label, start_info);
}

}  // namespace nn::atk
