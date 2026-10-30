#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/StreamSoundRuntime.h>

namespace nn::atk::detail {

// NON_MATCHING: the compiler moves the constant multiplication before the failure selection.
size_t StreamSoundRuntime::GetRequiredStreamBufferSize(const SoundArchive* archive) const {
    SoundArchive::SoundArchivePlayerInfo info;
    s64 channel_count = archive->ReadSoundArchivePlayerInfo(&info) ? info.streamChannelCount : 0;
    return _a8 * channel_count * 0x2900;
}

u32 StreamSoundRuntime::GetRequiredStreamBufferTimes(const SoundArchive* archive) {
    SoundArchive::SoundArchivePlayerInfo info;
    return archive->ReadSoundArchivePlayerInfo(&info) ? info.streamBufferTimes : 1;
}

size_t StreamSoundRuntime::GetRequiredStreamCacheSize(const SoundArchive* archive,
                                                     size_t cache_size) {
    SoundArchive::SoundArchivePlayerInfo info;
    return (archive->ReadSoundArchivePlayerInfo(&info) ? info.streamSoundCount : 0) * cache_size;
}

}  // namespace nn::atk::detail
