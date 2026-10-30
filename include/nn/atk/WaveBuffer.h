#pragma once

#include <nn/types.h>

namespace nn::atk {

// Partial record: the sound voice methods link buffers and update the state at +0x34.
struct WaveBuffer {
    u8 _0[0x34];
    u32 _34;
    WaveBuffer* mNext;
};

}  // namespace nn::atk
