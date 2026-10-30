#include <nn/atk/detail/driver/SequenceTrack.h>
#include <nn/atk/detail/driver/Channel.h>
#include <nn/atk/detail/driver/MultiVoice.h>

namespace nn::atk::detail::driver {

void SequenceTrack::FreeAllChannel() {
    for (Channel* channel = mChannelHead; channel; channel = channel->mNext)
        Channel::DetachChannel(channel);
    mChannelHead = nullptr;
}

void SequenceTrack::PauseAllChannel(bool pause) {
    for (Channel* channel = mChannelHead; channel; channel = channel->mNext) {
        if (channel->mActive && (channel->mPaused != 0) != pause) {
            channel->mPaused = pause;
            channel->mVoice->Pause(pause);
        }
    }
}

// NON_MATCHING: the compiler reorganizes the counter and parse-loop branches.
int SequenceTrack::ParseNextTick(bool flag) {
    if (!mIsOpen)
        return 0;

    if (_138 < _136)
        ++_138;
    if (_13e < _13c)
        ++_13e;
    if (_144 < _142)
        ++_144;
    if (_14a < _148)
        ++_14a;
    if (_150 < _14e)
        ++_150;

    if (_128) {
        if (mChannelHead)
            return 1;
        _128 = 0;
    }
    if (_124 > 0)
        --_124;
    if (!mSequenceCursor || _124 != 0)
        return 1;

    for (int i = 0; i < 10000; ++i) {
        if (_128)
            return 1;
        if (Parse(flag) == 1)
            return -1;
        if (_124 != 0)
            return 1;
    }
    return 1;
}

void SequenceTrack::Open() {
    _128 = 0;
    _120 = 0;
    _124 = 0;
    mIsOpen = true;
}

// 0x710134b540
s16* SequenceTrack::GetVariablePtr(int index) {
    return index < 0x10 ? &mTrackVariables[index] : nullptr;
}

void SequenceTrack::SetPlayerTrackNo(int track_no) {
    mPlayerTrackNo = track_no;
}

void SequenceTrack::SetSeqData(const void* data, int start_offset) {
    mSeqData = data;
    mSequenceCursor = static_cast<const u8*>(data) + start_offset;
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
