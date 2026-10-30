#pragma once

#include <nn/types.h>
#include <nn/atk/AuxBus.h>

namespace nn::atk {
enum SequenceMute : int;
}

namespace nn::atk::detail::driver {

// TODO: incomplete. Only the fields written by the modelled sequencer setters are named; the
// channel and tick state of the track is not modelled.
class SequenceTrack {
public:
    SequenceTrack();
    virtual ~SequenceTrack();
    // The full-word comparison in ParseNextTick supports a numeric status; int is inferred.
    virtual int Parse(bool flag) = 0;

    void Open();
    void SetMute(SequenceMute mute);
    void SetSilence(bool silence, int fade_frames);
    void SetPlayerTrackNo(int track_no);
    void SetSeqData(const void* data, int size);
    void SetTrackVariable(int index, s16 value);
    // 0x710134b540
    s16* GetVariablePtr(int index);
    void SetBankIndex(int index);
    void SetTranspose(int transpose);
    void SetVelocityRange(int range);
    void SetOutputLine(u32 line);
    // 0x710134b508
    void SetBiquadFilter(int type, f32 frequency);

    // Inline-only in the original; SequenceSoundPlayer's SetTrack* loops write these fields.
    void SetVolume(f32 volume) { mVolume = volume; }
    void SetPitch(f32 pitch) { mPitch = pitch; }
    void SetLpfFreq(f32 frequency) { mLpfFreq = frequency; }
    void SetTvVolume(f32 volume) { mTvVolume = volume; }
    void SetTvPan(f32 pan) { mTvPan = pan; }
    void SetTvSurroundPan(f32 pan) { mTvSurroundPan = pan; }
    void SetTvMainSend(f32 send) { mTvMainSend = send; }
    void SetTvFxSend(nn::atk::AuxBus bus, f32 send) { mTvFxSend[u32(bus)] = send; }

private:
    friend class SequenceSoundPlayer;

    /* 0x8 */ u8 mPlayerTrackNo;
    bool mIsOpen;
    u8 _a[0xc - 0xa];
    /* 0xc */ f32 mVolume;
    /* 0x10 */ f32 mPitch;
    /* 0x14 */ u32 _14;
    /* 0x18 */ f32 mTvVolume;
    u8 _1c[0x50 - 0x1c];
    /* 0x50 */ f32 mTvPan;
    /* 0x54 */ f32 mTvSurroundPan;
    /* 0x58 */ f32 mTvMainSend;
    /* 0x5c */ f32 mTvFxSend[3];
    /* 0x68 */ const void* mSeqData;
    /* 0x70 */ const void* mSeqDataEnd;
    u8 _78;
    u8 _79[0x120 - 0x79];
    u8 _120;
    u8 _121[3];
    s32 _124;
    u8 _128;
    u8 _129[2];
    /* 0x12b */ u8 mBankIndex;
    u8 _12c[0x198 - 0x12c];
    /* 0x198 */ u8 mVelocityRange;
    u8 _199[0x19c - 0x199];
    /* 0x19c */ u8 mTranspose;
    u8 _19d[0x1a6 - 0x19d];
    /* 0x1a6 */ u8 mBiquadFilterType;
    u8 _1a7[0x1ac - 0x1a7];
    /* 0x1ac */ f32 mLpfFreq;
    /* 0x1b0 */ f32 mBiquadFilterFrequency;
    /* 0x1b4 */ u32 mOutputLine;
    /* 0x1b8 */ s16 mTrackVariables[16];
    u8 _1d8[0x10];
};
static_assert(sizeof(SequenceTrack) == 0x1e8);

}  // namespace nn::atk::detail::driver
