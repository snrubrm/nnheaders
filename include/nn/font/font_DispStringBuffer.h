#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>

namespace nn::font {

// Partial layout: TextBox::AllocateStringBuffer allocates this 0xc0-byte object and extra GPU
// storage. Copying a TextBox reads the capacity at +0x98; per-character updates read +0x9c.
class DispStringBuffer {
public:
    struct UnkA0 {
        u8 _0[0x38];
    };
    struct InitializeArg {
        UnkA0* _0 = nullptr;
        void* _8 = nullptr;
        s32 mCapacity = 0;
        bool _14 = false;
        bool _15 = false;
        bool _16 = false;
    };
    ~DispStringBuffer();
    bool Initialize(gfx::Device*, const InitializeArg&);
    // Clears the initialized capacity; called before destruction by TextBox::FreeStringBuffer.
    void sub_7101324090(gfx::Device*);
    static size_t sub_71013240A4(const InitializeArg&);

    /* 0x00 */ u8 _0[0x98];
    /* 0x98 */ s32 mCapacity;
    /* 0x9c */ u32 mCharCount;
    /* 0xa0 */ UnkA0* _a0;
    /* 0xa8 */ u8* _a8;
    /* 0xb0 */ void* _b0;
    /* 0xb8 */ bool _b8;
    /* 0xb9 */ bool _b9;
    /* 0xba */ bool _ba;
};
static_assert(sizeof(DispStringBuffer::InitializeArg) == 0x18);
static_assert(sizeof(DispStringBuffer::UnkA0) == 0x38);
static_assert(sizeof(DispStringBuffer) == 0xc0);

}  // namespace nn::font
