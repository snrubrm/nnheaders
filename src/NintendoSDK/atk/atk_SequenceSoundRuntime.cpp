#include <nn/atk/detail/SequenceSoundRuntime.h>
#include <nn/atk/detail/driver/MmlSequenceTrack.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {

size_t SequenceSoundRuntime::GetRequiredMemorySize(
    const SoundArchive::SoundArchivePlayerInfo& info, int alignment) {
    // The two allocation strides are used by SoundInstanceManager::Create at 0x710134961c.
    return nn::util::align_up(0x560UL * info.sequenceSoundCount, alignment) +
           nn::util::align_up(0x4f8UL * info.sequenceSoundCount, alignment);
}

size_t SequenceSoundRuntime::GetRequiredSequenceTrackMemorySize(
    const SoundArchive::SoundArchivePlayerInfo& info, int alignment) {
    return nn::util::align_up(sizeof(driver::MmlSequenceTrack) * info.sequenceTrackCount,
                              alignment);
}

}  // namespace nn::atk::detail
