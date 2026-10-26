#pragma once

#include <nn/types.h>

namespace nn::atk {

// Partial model. Only the detach used by the runtime sound instance is declared.
class StreamSoundHandle {
public:
    void DetachSound();
};

}  // namespace nn::atk
