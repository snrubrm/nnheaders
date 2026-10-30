#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/StreamSoundRuntime.h>

namespace nn::atk::detail {

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
