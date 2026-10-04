#pragma once

#include <nn/types.h>

namespace nn::g3d {

// Only the callback argument is recovered. The callback interface is not
// declared until its full virtual contract is established.
class ICalculateBlendWeightCallback {
public:
    // The blend producers (0x13349a8 / 0x1334ad8) construct this index/weight
    // pair; gsys::ModelAnimation::Exec (0xbff26c) forwards it unchanged.
    struct CallbackArg {
        s32 bone_index;
        f32 weight;
    };
};
static_assert(sizeof(ICalculateBlendWeightCallback::CallbackArg) == 8);

}  // namespace nn::g3d
