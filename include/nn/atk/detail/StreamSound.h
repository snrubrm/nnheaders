#pragma once

#include <nn/atk/StreamSoundHandle.h>
#include <nn/atk/detail/BasicSoundPlayerHandle.h>
#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the temporary special handle at +0x1f0 and the basic player handle at
// +0x2c0 are modelled.
class StreamSound {
public:
    bool IsAttachedTempSpecialHandle();
    void DetachTempSpecialHandle();
    // 0x710134c490
    SoundPlayerHandle* GetBasicSoundPlayerHandle();

private:
    friend class nn::atk::StreamSoundHandle;

    u8 _0[0x1f0];
    /* 0x1f0 */ StreamSoundHandle* mTempSpecialHandle;
    u8 _1f8[0x2c0 - 0x1f8];
    /* 0x2c0 */ BasicSoundPlayerHandle mBasicSoundPlayerHandle;
};

}  // namespace nn::atk::detail
