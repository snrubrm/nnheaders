#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

class CommandManager;

// TODO: incomplete. Only the static memory-size query is modelled.
class LowLevelVoiceCommand {
public:
    // Native constructs and returns a CommandManager; return type is inferred.
    static CommandManager& GetInstance();
    static size_t GetRequiredWaveBufferMemSize(int voice_count);
};

}  // namespace nn::atk::detail
