#pragma once

#include <nn/types.h>
#include <nn/atk/BiquadFilterCallback.h>

namespace nn::atk::detail {

// TODO: incomplete. The filter parameters and playback cursor are modelled; the constructor
// writes -1 to the s32 at +0xac.
class Voice {
public:
    Voice();
    // 0x7101358bd4: the original body is a four-byte empty function.
    ~Voice();
    u64 GetPlayPosition() const;
    bool IsAvailable() const;
    void SetMonoFilter(bool enabled, u16 coefficient);
    void SetBiquadFilter(bool enabled, const BiquadFilterCoefficients* coefficients);

private:
    u8 _0[0x70];
    /* 0x70 */ bool mMonoFilterEnabled;
    /* 0x71 */ bool mBiquadFilterEnabled;
    /* 0x72 */ BiquadFilterCoefficients mBiquadFilterCoefficients;
    /* 0x7c */ u16 mMonoFilterCoefficient;
    u8 _7e[0xac - 0x7e];
    /* 0xac */ s32 _ac;
    /* 0xb0 */ u64 mPlayPosition;
    u8 _b8[0xd8 - 0xb8];
};

static_assert(sizeof(Voice) == 0xd8);

}  // namespace nn::atk::detail
