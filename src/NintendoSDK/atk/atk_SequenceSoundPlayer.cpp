#include <nn/atk/detail/driver/SequenceSoundPlayer.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

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
