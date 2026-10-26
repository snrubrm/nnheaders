#include <nn/atk/detail/driver/SequenceTrack.h>

namespace nn::atk::detail::driver {

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

}  // namespace nn::atk::detail::driver
