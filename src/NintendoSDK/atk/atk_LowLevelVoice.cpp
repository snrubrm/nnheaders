#include <nn/atk/detail/LowLevelVoice.h>
#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn::atk::detail {

bool LowLevelVoice::IsAvailable() const {
    return mAvailable;
}

// NON_MATCHING: the compiler combines the array sizes before multiplying the voice count.
size_t LowLevelVoiceAllocator::GetRequiredMemSize(int max_voices) {
    return max_voices * (sizeof(LowLevelVoice) + sizeof(LowLevelVoice*) + sizeof(s32)) + 0x40;
}

}  // namespace nn::atk::detail
