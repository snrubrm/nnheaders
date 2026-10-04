#pragma once

#include <nn/types.h>

namespace nn::font {

// Partial layout: TextBox::AllocateStringBuffer allocates this 0xc0-byte object and extra GPU
// storage. Copying a TextBox reads the capacity at +0x98; per-character updates read +0x9c.
class DispStringBuffer {
public:
    /* 0x00 */ u8 _0[0x98];
    /* 0x98 */ u32 mCapacity;
    /* 0x9c */ u32 mCharCount;
    /* 0xa0 */ u8 _a0[0x20];
};
static_assert(sizeof(DispStringBuffer) == 0xc0);

}  // namespace nn::font
