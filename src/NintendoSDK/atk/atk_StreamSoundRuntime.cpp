#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/StreamSoundRuntime.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {

// NON_MATCHING: the two aligned size calculations are scheduled in reverse order.
size_t StreamSoundRuntime::GetRequiredMemorySize(
    const SoundArchive::SoundArchivePlayerInfo& info, int alignment) {
    // Initialize 0x71013500c8 allocates these two per-sound strides.
    return nn::util::align_up(0x35c0UL * info.streamSoundCount, alignment) +
           nn::util::align_up(0x119c0UL * info.streamSoundCount, 0x1000);
}

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
