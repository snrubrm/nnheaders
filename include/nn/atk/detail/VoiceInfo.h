#pragma once

#include <nn/types.h>

namespace nn::atk {
struct WaveBuffer;
}

namespace nn::atk::detail {

class Voice;

struct VoiceInfo {
    u32 _0;
    u32 mWaveBufferState;
    WaveBuffer* mWaveBuffer;
    u32 mPlayPosition;
    u8 _14[4];
    Voice* mVoice;
};
static_assert(sizeof(VoiceInfo) == 0x20);

}  // namespace nn::atk::detail
