#include <nn/atk/detail/driver/SequenceTrack.h>

namespace nn::atk::detail::driver {

// 0x710134b540
s16* SequenceTrack::GetVariablePtr(int index) {
    return index < 0x10 ? &mTrackVariables[index] : nullptr;
}

void SequenceTrack::SetPlayerTrackNo(int track_no) {
    mPlayerTrackNo = track_no;
}

void SequenceTrack::SetSeqData(const void* data, int size) {
    mSeqData = data;
    mSeqDataEnd = static_cast<const u8*>(data) + size;
}

void SequenceTrack::SetTrackVariable(int index, s16 value) {
    mTrackVariables[index] = value;
}

void SequenceTrack::SetBankIndex(int index) {
    mBankIndex = index;
}

void SequenceTrack::SetTranspose(int transpose) {
    mTranspose = transpose;
}

void SequenceTrack::SetVelocityRange(int range) {
    mVelocityRange = range;
}

void SequenceTrack::SetOutputLine(u32 line) {
    mOutputLine = line;
}

void SequenceTrack::SetBiquadFilter(int type, f32 frequency) {
    mBiquadFilterType = type;
    mBiquadFilterFrequency = frequency;
}

}  // namespace nn::atk::detail::driver
