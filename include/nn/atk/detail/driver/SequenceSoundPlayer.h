#pragma once

#include <nn/types.h>
#include <nn/atk/AuxBus.h>
#include <nn/atk/detail/BankFileReader.h>

namespace nn {
namespace atk {

enum SequenceMute : int;

/// The parameters the sequence player passes to the user's per-tick callback.
struct SequenceUserProcCallbackParam {
    s16* mPlayerVariables;
    s16* mGlobalVariables;
    s16* mTrackVariables;
    u8 _18;
};

namespace detail {
namespace driver {

class Channel;
class SequenceTrack;

/// The user callback the player runs at every sequence tick.
using SequenceUserProcCallback = void (*)(u16, SequenceUserProcCallbackParam*, void*);

/// Partial model: the player is polymorphic and derives from the sound driver's player base, so
/// only the fields the setters below write are placed, at the offsets their bodies give.
class SequenceSoundPlayer {
public:
    SequenceSoundPlayer();  // 0x7101347490, declared only
    static void InitSequenceSoundPlayer();
    enum StartOffsetType {
        StartOffsetType_0,
        StartOffsetType_1,
    };

    void Skip(StartOffsetType type, int amount);
    void CallSequenceUserprocCallback(u16 value, SequenceTrack* track);
    void SetTrackMute(u32 track_mask, SequenceMute mute);
    void SetTrackSilence(u64 track_mask, bool silence, int fade_frames);
    bool SetTrackBankIndex(u32 track_mask, int bank_index);
    void SetTempoRatio(f32 ratio);
    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool fix);
    void SetSequenceUserprocCallback(SequenceUserProcCallback callback, void* arg);
    void SetLocalVariable(int index, s16 value);
    static void SetGlobalVariable(int variable, s16 value);
    s16* GetVariablePtr(int variable);

    // 0x7101347d00 / 0x7101347f1c .. 0x7101348438
    SequenceTrack* GetPlayerTrack(int index);
    void SetTrackVolume(u32 track_mask, f32 volume);
    void SetTrackPitch(u32 track_mask, f32 pitch);
    void SetTrackLpfFreq(u32 track_mask, f32 frequency);
    void SetTrackTvVolume(u32 track_mask, f32 volume);
    void SetTrackTvPan(u32 track_mask, f32 pan);
    void SetTrackTvSurroundPan(u32 track_mask, f32 pan);
    void SetTrackTvMainSend(u32 track_mask, f32 send);
    void SetTrackBiquadFilter(u32 track_mask, int type, f32 frequency);
    void SetTrackTranspose(u32 track_mask, s8 transpose);
    void SetTrackVelocityRange(u32 track_mask, u8 range);
    void SetTrackOutputLine(u32 track_mask, u32 line);
    void ResetTrackOutputLine(u32 track_mask);
    void SetTrackTvFxSend(u32 track_mask, nn::atk::AuxBus bus, f32 send);

    // The state transitions. In the original these are virtual overrides; their vtable slots are
    // not established here, so they are plain members.
    void Start();
    void Update();
    void ChannelCallback(Channel* channel);
    void OnUpdateFrameSoundThread();
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency();

private:
    u8 _0[0x30];
    bool _30;
    bool mStarted;  // +0x31
    u8 _32[0xe0 - 0x32];
    bool mReleasePriorityFix;  // +0xe0
    u8 _e1[0xe8 - 0xe1];
    f32 mTempoRatio;           // +0xe8
    u8 _ec[4];
    s32 _f0;
    f32 _f4;
    u8 _f8[8];
    u8 mChannelPriority;       // +0x100
    u8 _101[0x120 - 0x101];
    SequenceUserProcCallback mUserProcCallback;  // +0x120
    void* mUserProcArg;                          // +0x128
    SequenceTrack* mTracks[16];  // +0x130
    s16 mLocalVariables[0x20];                   // +0x1b0, indexed by the variable number
    u8 _1f0[0x258 - 0x1f0];
    BankFileReader mBankReaders[4];  // +0x258, constructed/finalized individually in the native player
    u8 _2b8[0x350 - 0x2b8];
    u32 mAudioFrameFrequency;                    // +0x350, zero means one update per frame

    // The variables shared by every player; variable numbers 16..31 select an entry here.
    static s16 sGlobalVariable[0x10];
};

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn
