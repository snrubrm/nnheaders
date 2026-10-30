#pragma once

#include <nn/types.h>

namespace nn::atk {

// Partial record: the sound voice methods link buffers and update the state at +0x34.
struct WaveBuffer {
    u8 _0[0x28];
    WaveBuffer* mSource;  // +0x28: the original buffer associated with this low-level buffer.
    u8 _30[4];
    u32 _34;
    WaveBuffer* mNext;
};

}  // namespace nn::atk
