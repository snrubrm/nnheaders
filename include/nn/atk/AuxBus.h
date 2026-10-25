#pragma once

#include <nn/types.h>

namespace nn {
namespace atk {
// Unsigned: BasicSound::SetFxSend 0x710133fa90 indexes the send array with
// `add x8, x0, w1, uxtw #2`, so the native conversion zero-extends the bus value.
enum AuxBus : u32 { AuxBus_A, AuxBus_B, AuxBus_C };
}
}  // namespace nn
