#pragma once

#include <nn/types.h>

namespace nn::atk {

/// The coefficients of a biquad filter.
struct BiquadFilterCoefficients {
    s16 b0;
    s16 b1;
    s16 b2;
    s16 a1;
    s16 a2;
};
static_assert(sizeof(BiquadFilterCoefficients) == 10);

/// Provides the coefficients of the biquad filters of an audio engine.
class IBiquadFilterCallback {
public:
    virtual ~IBiquadFilterCallback() = default;
    virtual void GetCoefficients(BiquadFilterCoefficients* coefficients, int type,
                                 float value) const = 0;
};

}  // namespace nn::atk
