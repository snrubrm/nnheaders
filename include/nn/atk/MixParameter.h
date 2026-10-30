#pragma once

#include <nn/types.h>

namespace nn::atk {

// Six channel mix factors; individual names are not recovered.
struct MixParameter {
    f32 _0[6];
};
static_assert(sizeof(MixParameter) == 0x18);

}  // namespace nn::atk
