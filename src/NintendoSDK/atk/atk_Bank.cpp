#include <nn/atk/detail/driver/Bank.h>

namespace nn::atk::detail::driver {

Bank::Bank() {
}

Bank::~Bank() {
}

f32 Bank::CalcChannelVelocityVolume(u8 velocity) {
    return velocity * (1.0f / 127.0f);
}

}  // namespace nn::atk::detail::driver
