#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the temporary special handle at +0x1f0 is modelled.
class SequenceSound {
public:
    bool IsAttachedTempSpecialHandle();

private:
    u8 _0[0x1f0];
    /* 0x1f0 */ const void* mTempSpecialHandle;
};

}  // namespace nn::atk::detail
