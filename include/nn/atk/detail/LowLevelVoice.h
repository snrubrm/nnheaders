#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the available flag at +0x28 is modelled.
class LowLevelVoice {
public:
    bool IsAvailable() const;

private:
    u8 _0[0x28];
    /* 0x28 */ bool mAvailable;
};

}  // namespace nn::atk::detail
