#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the static memory-size query is modelled.
class LowLevelVoiceCommand {
public:
    static size_t GetRequiredWaveBufferMemSize(int voice_count);
};

}  // namespace nn::atk::detail
