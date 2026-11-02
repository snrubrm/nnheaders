#include <nn/atk/detail/AdvancedWaveSoundRuntime.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {

size_t AdvancedWaveSoundRuntime::GetRequiredMemorySize(
    const SoundArchive::SoundArchivePlayerInfo& info, size_t alignment) {
    // Initialize 0x710133e55c allocates this per-sound stride.
    return nn::util::align_up(0x970UL * info.waveSoundCount, alignment);
}

}  // namespace nn::atk::detail
