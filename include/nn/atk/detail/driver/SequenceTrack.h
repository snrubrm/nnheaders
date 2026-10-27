#pragma once

#include <nn/types.h>

namespace nn::atk::detail::driver {

// TODO: incomplete. Only the fields written by the modelled sequencer setters are named; the
// channel and tick state of the track is not modelled.
class SequenceTrack {
public:
    void SetPlayerTrackNo(int track_no);
    void SetSeqData(const void* data, int size);
    void SetTrackVariable(int index, s16 value);
    void SetBankIndex(int index);
    void SetTranspose(int transpose);
    void SetVelocityRange(int range);
    void SetOutputLine(u32 line);
    // 0x710134b508
    void SetBiquadFilter(int type, f32 frequency);

private:
    u8 _0[8];
    /* 0x8 */ u8 mPlayerTrackNo;
    u8 _9[0x68 - 0x9];
    /* 0x68 */ const void* mSeqData;
    /* 0x70 */ const void* mSeqDataEnd;
    u8 _78[0x12b - 0x78];
    /* 0x12b */ u8 mBankIndex;
    u8 _12c[0x198 - 0x12c];
    /* 0x198 */ u8 mVelocityRange;
    u8 _199[0x19c - 0x199];
    /* 0x19c */ u8 mTranspose;
    u8 _19d[0x1a6 - 0x19d];
    /* 0x1a6 */ u8 mBiquadFilterType;
    u8 _1a7[0x1b0 - 0x1a7];
    /* 0x1b0 */ f32 mBiquadFilterFrequency;
    /* 0x1b4 */ u32 mOutputLine;
    /* 0x1b8 */ s16 mTrackVariables[16];
};

}  // namespace nn::atk::detail::driver
