#pragma once

#include <nn/types.h>
#include <nn/audio.h>
#include <nn/atk/OutputMode.h>

namespace nn::atk {
struct WaveBuffer;
}

namespace nn::atk::detail {

class Voice;
struct VoiceInfo;

// Partial names inferred from SetState's acquire and release behavior.
enum VoiceState : s32 {
    VoiceState_Acquired = 0,
    VoiceState_Released = 1,
};

// TODO: incomplete. The renderer voice, allocation state and wave-buffer links are modelled;
// the remaining spans stay opaque.
class LowLevelVoice {
public:
    bool IsAvailable() const;
    void SetPriority(int priority);
    void SetState(VoiceState state);
    void AppendWaveBuffer(WaveBuffer* buffer);
    void FreeAllWaveBuffer();
    void UpdateVoiceInfo(VoiceInfo* info) const;

private:
    friend class LowLevelVoiceAllocator;
    void AllocVoice();
    // The allocator discards the native result; void is inferred.
    void UpdateState(OutputMode mode);
    u8 _0[0x20];
    nn::audio::VoiceType mVoice;  // +0x20
    /* 0x28 */ bool mAvailable;
    bool mHasVoiceSlot;             // +0x29
    u8 _2a[0xa4 - 0x2a];
    s32 mPriority;                 // +0xa4: SetPriority 0x7101355f78
    VoiceState mState;             // +0xa8
    s32 mSampleRate;               // +0xac
    s32 _b0;
    u8 _b4[4];
    u64 _b8;
    s32 _c0;
    u8 _c4[4];
    WaveBuffer* mWaveBufferHead;    // +0xc8
    WaveBuffer* mWaveBufferTail;    // +0xd0
    WaveBuffer* mWaveBufferCursor;  // +0xd8
    Voice* mSourceVoice;            // +0xe0
    u32 _e8;
    u8 _ec;
    // The allocator's initialization loop at 0x7101356d84 uses a 0x100-byte stride.
    u8 _ed[0x100 - 0xed];
};
static_assert(sizeof(LowLevelVoice) == 0x100);

}  // namespace nn::atk::detail
