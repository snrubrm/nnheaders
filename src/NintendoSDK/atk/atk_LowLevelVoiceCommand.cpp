#include <nn/atk/detail/LowLevelVoiceCommand.h>

namespace nn::atk::detail {

size_t LowLevelVoiceCommand::GetRequiredWaveBufferMemSize(int voice_count) {
    return static_cast<size_t>(voice_count) * 128;
}

}  // namespace nn::atk::detail
