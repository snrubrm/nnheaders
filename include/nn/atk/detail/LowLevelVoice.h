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
    // The allocator's initialization loop at 0x7101356d84 uses a 0x100-byte stride.
    u8 _29[0x100 - 0x29];
};
static_assert(sizeof(LowLevelVoice) == 0x100);

}  // namespace nn::atk::detail
