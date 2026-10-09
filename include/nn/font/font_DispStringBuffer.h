#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>

namespace nn::font {

class TextureObject;
class GpuBuffer;

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
    DispStringBuffer();
    ~DispStringBuffer();
    bool Initialize(gfx::Device*, const InitializeArg&);
    // Clears the initialized capacity; called before destruction by TextBox::FreeStringBuffer.
    void sub_7101324090(gfx::Device*);
    static size_t sub_71013240A4(const InitializeArg&);
    static size_t sub_71013240B4(gfx::Device*, const InitializeArg&);
    void SetGpuBuffer(GpuBuffer* buffer);
    void SetFontHeight(f32 height);

    // BuildConstantBuffer and the drawing helper keep byte offsets at 0/4, flags at8,
    // and up to eight texture/count/flag records at10. Names are inferred from those uses.
    struct TextureEntry {
        const TextureObject* mTexture = nullptr;
        u32 mCount = 0;
        u32 mFlags = 0;
    };
    static_assert(sizeof(TextureEntry) == 0x10);

    /* 0x00 */ u32 mConstantBufferOffset = 0;
    /* 0x04 */ u32 mVertexBufferOffset = 0;
    /* 0x08 */ u32 mFlags = 0;
    /* 0x10 */ TextureEntry mTextures[8]{};
    /* 0x90 */ s32 mTextureCount = 0;
    /* 0x94 */ u8 _94[4]{};
    /* 0x98 */ s32 mCapacity = 0;
    /* 0x9c */ u32 mCharCount = 0;
    /* 0xa0 */ UnkA0* _a0 = nullptr;
    /* 0xa8 */ u8* _a8 = nullptr;
    /* 0xb0 */ void* _b0 = nullptr;
    /* 0xb8 */ bool _b8 = false;
    /* 0xb9 */ bool _b9 = false;
    /* 0xba */ bool _ba = false;
    /* 0xbc */ f32 mFontHeight;
};
static_assert(sizeof(DispStringBuffer::InitializeArg) == 0x18);
static_assert(sizeof(DispStringBuffer::UnkA0) == 0x38);
static_assert(sizeof(DispStringBuffer) == 0xc0);

}  // namespace nn::font
