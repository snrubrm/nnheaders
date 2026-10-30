#pragma once

#include <nn/types.h>

namespace nn::atk::detail::driver {

class Bank {
public:
    Bank();
    ~Bank();

    static f32 CalcChannelVelocityVolume(u8 velocity);
};

}  // namespace nn::atk::detail::driver
