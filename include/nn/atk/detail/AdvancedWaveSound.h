#pragma once

#include <nn/atk/detail/BasicSoundPlayerHandle.h>
#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the temporary special handle at +0x1f0 and the basic player handle at
// +0x200 are modelled.
class AdvancedWaveSound {
public:
    bool IsAttachedTempSpecialHandle();
    void DetachTempSpecialHandle();
    // 0x710135dfd8
    SoundPlayerHandle* GetBasicSoundPlayerHandle();

private:
    u8 _0[0x1f0];
    /* 0x1f0 */ const void* mTempSpecialHandle;
    u8 _1f8[0x200 - 0x1f8];
    /* 0x200 */ BasicSoundPlayerHandle mBasicSoundPlayerHandle;
};

}  // namespace nn::atk::detail
