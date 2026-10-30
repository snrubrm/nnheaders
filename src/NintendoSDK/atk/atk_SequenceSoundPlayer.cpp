#include <nn/atk/detail/driver/SequenceSoundPlayer.h>
#include <nn/atk/detail/driver/SequenceTrack.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

// NON_MATCHING: the compiler selects the two offset cases in the opposite order.
void SequenceSoundPlayer::Skip(StartOffsetType type, int amount) {
    if (_30) {
        if (type == StartOffsetType_1)
            _f4 += amount;
        else if (type == StartOffsetType_0)
            _f0 += amount;
    }
}

void SequenceSoundPlayer::SetTempoRatio(f32 ratio) {
    mTempoRatio = ratio;
}

void SequenceSoundPlayer::SetChannelPriority(int priority) {
    mChannelPriority = priority;
}

void SequenceSoundPlayer::SetReleasePriorityFix(bool fix) {
    mReleasePriorityFix = fix;
}

void SequenceSoundPlayer::SetSequenceUserprocCallback(SequenceUserProcCallback callback, void* arg) {
    mUserProcCallback = callback;
    mUserProcArg = arg;
}

void SequenceSoundPlayer::SetLocalVariable(int index, s16 value) {
    mLocalVariables[index] = value;
}

void SequenceSoundPlayer::SetGlobalVariable(int variable, s16 value) {
    sGlobalVariable[variable] = value;
}

SequenceTrack* SequenceSoundPlayer::GetPlayerTrack(int index) {
    if (index > 0xf)
        return nullptr;
    return mTracks[index];
}

void SequenceSoundPlayer::SetTrackVolume(u32 track_mask, f32 volume) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetVolume(volume);
        }
    }
}

void SequenceSoundPlayer::SetTrackPitch(u32 track_mask, f32 pitch) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetPitch(pitch);
        }
    }
}

void SequenceSoundPlayer::SetTrackLpfFreq(u32 track_mask, f32 frequency) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetLpfFreq(frequency);
        }
    }
}

void SequenceSoundPlayer::SetTrackTvVolume(u32 track_mask, f32 volume) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetTvVolume(volume);
        }
    }
}

void SequenceSoundPlayer::SetTrackTvPan(u32 track_mask, f32 pan) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetTvPan(pan);
        }
    }
}

void SequenceSoundPlayer::SetTrackTvSurroundPan(u32 track_mask, f32 pan) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetTvSurroundPan(pan);
        }
    }
}

void SequenceSoundPlayer::SetTrackTvMainSend(u32 track_mask, f32 send) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetTvMainSend(send);
        }
    }
}

void SequenceSoundPlayer::SetTrackBiquadFilter(u32 track_mask, int type, f32 frequency) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetBiquadFilter(type, frequency);
        }
    }
}

// NON_MATCHING: the original passes the signed char through without the sign extension our clang
// emits, which also changes the callee-saved register colours (96 vs 92 bytes).
void SequenceSoundPlayer::SetTrackTranspose(u32 track_mask, s8 transpose) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetTranspose(transpose);
        }
    }
}

// NON_MATCHING: the original passes the unsigned char through without the zero extension our clang
// emits, which also changes the callee-saved register colours (96 vs 92 bytes).
void SequenceSoundPlayer::SetTrackVelocityRange(u32 track_mask, u8 range) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetVelocityRange(range);
        }
    }
}

void SequenceSoundPlayer::SetTrackOutputLine(u32 track_mask, u32 line) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetOutputLine(line);
        }
    }
}

void SequenceSoundPlayer::ResetTrackOutputLine(u32 track_mask) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetOutputLine(-1);
        }
    }
}

void SequenceSoundPlayer::SetTrackTvFxSend(u32 track_mask, nn::atk::AuxBus bus, f32 send) {
    for (s32 i = 0; i < 16 && track_mask != 0; ++i, track_mask >>= 1) {
        if ((track_mask & 1) != 0) {
            if (SequenceTrack* track = mTracks[i])
                track->SetTvFxSend(bus, send);
        }
    }
}

s16* SequenceSoundPlayer::GetVariablePtr(int variable) {
    if (variable <= 0xf)
        return &mLocalVariables[variable];
    if (variable <= 0x1f)
        return &sGlobalVariable[variable - 0x10];
    return nullptr;
}

s16 SequenceSoundPlayer::sGlobalVariable[0x10];

void SequenceSoundPlayer::Start() {
    mStarted = true;
}

void SequenceSoundPlayer::ChannelCallback(Channel*) {
}

void SequenceSoundPlayer::OnUpdateFrameSoundThread() {
    Update();
}

void SequenceSoundPlayer::OnUpdateFrameSoundThreadWithAudioFrameFrequency() {
    if (mAudioFrameFrequency == 0)
        Update();
}

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn
