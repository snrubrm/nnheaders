#pragma once

#include <nn/types.h>
#include <nn/util/util_BinTypes.h>

namespace nn::g3d {

// Model initialization C6DD78 advances the SDK shape array by 0x70.
class ResShape {
public:
    // Inline-only in the original; name is a guess. Independent C6F3CC and
    // 1200AC0 read this binary string for display-list labels and callback selection.
    const char* GetName() const { return pName.Get()->GetData(); }

    u8 _0[0x10];
    nn::util::BinPtrToString pName;
    u8 _18[0x70 - 0x18];
};
static_assert(sizeof(ResShape) == 0x70);
}  // namespace nn::g3d
