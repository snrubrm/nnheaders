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

    // Inline-only in the original; name is ours. The tera shader binding functions
    // (0x71011413b4, 0x7101141b28, 0x710114251c, 0x7101143c1c, 0x7101144ef4,
    // 0x71011456f0) load a halfword at +0x5e and compare it with an attribute index.
    u16 get_5e() const { return m_5e; }

    u8 _0[0x10];
    nn::util::BinPtrToString pName;
    u8 _18[0x5e - 0x18];
    u16 m_5e;
    u8 _60[0x70 - 0x60];
};
static_assert(sizeof(ResShape) == 0x70);
}  // namespace nn::g3d
