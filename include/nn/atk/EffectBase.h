#pragma once

#include <nn/types.h>

namespace nn::atk {

// TODO: incomplete. Only the sample rate at +0x1c is modelled.
class EffectBase {
public:
    s32 GetSampleRate() const;

private:
    u8 _0[0x1c];
    /* 0x1c */ s32 mSampleRate;
};

}  // namespace nn::atk
