#include <nn/atk/detail/SequenceSoundRuntime.h>
#include <nn/atk/detail/driver/MmlSequenceTrack.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {

size_t SequenceSoundRuntime::GetRequiredSequenceTrackMemorySize(
    const SoundArchive::SoundArchivePlayerInfo& info, int alignment) {
    return nn::util::align_up(sizeof(driver::MmlSequenceTrack) * info.sequenceTrackCount,
                              alignment);
}

}  // namespace nn::atk::detail
