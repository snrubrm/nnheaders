#pragma once

#include <nn/types.h>
#include <nn/audio.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the available flag at +0x28 is modelled.
class LowLevelVoice {
public:
    bool IsAvailable() const;
    void SetPriority(int priority);

private:
    friend class LowLevelVoiceAllocator;
    u8 _0[0x20];
    nn::audio::VoiceType mVoice;  // +0x20
    /* 0x28 */ bool mAvailable;
    u8 _29[0xa4 - 0x29];
    s32 mPriority;                 // +0xa4: SetPriority 0x7101355f78
    u8 _a8[4];
    s32 mSampleRate;               // +0xac
    s32 _b0;
    u8 _b4[4];
    u64 _b8;
    s32 _c0;
    u8 _c4[0xe8 - 0xc4];
    u32 _e8;
    u8 _ec;
    // The allocator's initialization loop at 0x7101356d84 uses a 0x100-byte stride.
    u8 _ed[0x100 - 0xed];
};
static_assert(sizeof(LowLevelVoice) == 0x100);

}  // namespace nn::atk::detail
