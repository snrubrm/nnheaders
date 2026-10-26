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

private:
    u8 _0[8];
    /* 0x8 */ u8 mPlayerTrackNo;
    u8 _9[0x68 - 0x9];
    /* 0x68 */ const void* mSeqData;
    /* 0x70 */ const void* mSeqDataEnd;
    u8 _78[0x1b8 - 0x78];
    /* 0x1b8 */ s16 mTrackVariables[16];
};

}  // namespace nn::atk::detail::driver
