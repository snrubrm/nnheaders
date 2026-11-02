#include <nn/atk/detail/WaveSoundRuntime.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {

size_t WaveSoundRuntime::GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& info,
                                               int alignment) {
    // Initialize 0x710135aec0 allocates these two per-sound strides.
    return nn::util::align_up(0x3a0UL * info.waveSoundCount, alignment) +
           nn::util::align_up(0x448UL * info.waveSoundCount, alignment);
}

}  // namespace nn::atk::detail
