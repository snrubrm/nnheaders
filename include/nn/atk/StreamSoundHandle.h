#pragma once

#include <nn/types.h>

namespace nn::atk {

namespace detail {
class StreamSound;
}

// Partial model. Only the detach used by the runtime sound instance is declared.
class StreamSoundHandle {
public:
    void DetachSound();

private:
    detail::StreamSound* mSound;
};

}  // namespace nn::atk
