#pragma once

#include <nn/atk/detail/BasicSoundPlayerHandle.h>
#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the temporary special handle at +0x1f0 and the basic player handle at
// +0x210 are modelled.
class WaveSound {
public:
    bool IsAttachedTempSpecialHandle();
    // 0x7101359f58
    SoundPlayerHandle* GetBasicSoundPlayerHandle();

private:
    u8 _0[0x1f0];
    /* 0x1f0 */ const void* mTempSpecialHandle;
    u8 _1f8[0x210 - 0x1f8];
    /* 0x210 */ BasicSoundPlayerHandle mBasicSoundPlayerHandle;
};

}  // namespace nn::atk::detail
