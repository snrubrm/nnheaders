#pragma once

#include <nn/atk/SequenceSoundHandle.h>
#include <nn/atk/detail/BasicSoundPlayerHandle.h>
#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the temporary special handle at +0x1f0 and the basic player handle at
// +0x208 are modelled.
class SequenceSound {
public:
    bool IsAttachedTempSpecialHandle();
    void DetachTempSpecialHandle();
    // 0x7101359f60 / 0x7101361c64: the native bodies are empty.
    void OnUpdateParam();
    // 0x7101361c5c
    SoundPlayerHandle* GetBasicSoundPlayerHandle();

private:
    u8 _0[0x1f0];
    /* 0x1f0 */ SequenceSoundHandle* mTempSpecialHandle;
    u8 _1f8[0x208 - 0x1f8];
    /* 0x208 */ BasicSoundPlayerHandle mBasicSoundPlayerHandle;
};

}  // namespace nn::atk::detail
