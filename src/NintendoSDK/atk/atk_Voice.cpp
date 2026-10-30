#include <nn/atk/detail/Voice.h>

namespace nn::atk::detail {

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
