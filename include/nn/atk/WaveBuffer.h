#pragma once

#include <nn/types.h>

namespace nn::atk {

// Partial record: the sound voice methods link buffers and update the state at +0x34.
struct WaveBuffer {
    u8 _0[0x10];
    // Signed width inferred from Channel's sample-position comparison; WaveInfo readers
    // populate this from a zero-extended 32-bit sample count.
    s64 _10;
    u8 _18[0x28 - 0x18];
    WaveBuffer* mSource;  // +0x28: the original buffer associated with this low-level buffer.
    u8 _30[4];
    u32 _34;
    WaveBuffer* mNext;
};
static_assert(sizeof(WaveBuffer) == 0x40);

}  // namespace nn::atk
