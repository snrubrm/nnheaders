#include <nn/atk/detail/Voice.h>
#include <nn/atk/detail/LowLevelVoice.h>
#include <nn/atk/detail/VirtualVoiceManager.h>

namespace nn::atk::detail {

// NON_MATCHING: the compiler uses GetInstance's return value instead of reloading the singleton.
bool Voice::IsAvailable() const {
    if (_ac == -1)
        return false;
    const LowLevelVoice* voice = VirtualVoiceManager::GetInstance().GetLowLevelVoice(_ac);
    return voice == nullptr || voice->IsAvailable();
}

void Voice::SetMonoFilter(bool enabled, u16 coefficient) {
    mMonoFilterEnabled = enabled;
    if (enabled)
        mMonoFilterCoefficient = coefficient;
}

void Voice::SetBiquadFilter(bool enabled, const BiquadFilterCoefficients* coefficients) {
    mBiquadFilterEnabled = enabled;
    if (enabled)
        mBiquadFilterCoefficients = *coefficients;
}

// 0x7101358bd4
Voice::~Voice() {}

Voice::Voice() {
    _ac = -1;
}

u64 Voice::GetPlayPosition() const {
    return mPlayPosition;
}

}  // namespace nn::atk::detail
