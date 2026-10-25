#pragma once

#include <nn/types.h>

namespace nn {
namespace atk {

/// The parameters the sequence player passes to the user's per-tick callback. The layout is not
/// known; the callback only receives a pointer to it.
struct SequenceUserProcCallbackParam;

namespace detail {
namespace driver {

class Channel;

/// The user callback the player runs at every sequence tick.
using SequenceUserProcCallback = void (*)(u16, SequenceUserProcCallbackParam*, void*);

/// Partial model: the player is polymorphic and derives from the sound driver's player base, so
/// only the fields the setters below write are placed, at the offsets their bodies give.
class SequenceSoundPlayer {
public:
    void SetTempoRatio(f32 ratio);
    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool fix);
    void SetSequenceUserprocCallback(SequenceUserProcCallback callback, void* arg);
    void SetLocalVariable(int index, s16 value);
    static void SetGlobalVariable(int variable, s16 value);
    s16* GetVariablePtr(int variable);

    // The state transitions. In the original these are virtual overrides; their vtable slots are
    // not established here, so they are plain members.
    void Start();
    void Update();
    void ChannelCallback(Channel* channel);
    void OnUpdateFrameSoundThread();
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency();

private:
    u8 _0[0x31];
    bool mStarted;  // +0x31
    u8 _32[0xe0 - 0x32];
    bool mReleasePriorityFix;  // +0xe0
    u8 _e1[0xe8 - 0xe1];
    f32 mTempoRatio;           // +0xe8
    u8 _ec[0x100 - 0xec];
    u8 mChannelPriority;       // +0x100
    u8 _101[0x120 - 0x101];
    SequenceUserProcCallback mUserProcCallback;  // +0x120
    void* mUserProcArg;                          // +0x128
    u8 _130[0x1b0 - 0x130];
    s16 mLocalVariables[0x20];                   // +0x1b0, indexed by the variable number
    u8 _1f0[0x350 - 0x1f0];
    u32 mAudioFrameFrequency;                    // +0x350, zero means one update per frame

    // The variables shared by every player; variable numbers 16..31 select an entry here.
    static s16 sGlobalVariable[0x10];
};

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn
