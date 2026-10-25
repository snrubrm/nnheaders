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

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn
