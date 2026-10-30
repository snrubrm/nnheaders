#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>
#include <nn/util/MathTypes.h>

namespace nn::font {

class TextureObject;
class GpuBuffer;
struct ShadowParameter;
// TextBox allocates the raw first record at a 0x2c stride and passes it unchanged
// through additional content to BuildPerCharacterParams. Names follow the observed targets.
struct PerCharacterTransformInfo {
    util::Float3 mRotationCos{{1.0f, 1.0f, 1.0f}};
    util::Float3 mRotationSin{{0.0f, 0.0f, 0.0f}};
    util::Float3 mTranslation{{0.0f, 0.0f, 0.0f}};
    util::Unorm8x4 mColors[2]{{{255, 255, 255, 255}}, {{255, 255, 255, 255}}};
};
static_assert(sizeof(PerCharacterTransformInfo) == 0x2c);

// BuildConstantBuffer copies 0x78 bytes. TextBox::BuildConstantBuffer supplies the two
// matrices; SetupConstantBufferAdditionalContent supplies the float colors and byte alphas.
// Unknown prefix semantics and padding remain unnamed; no default lifetime is implied.
struct ConstantBufferAdditionalContent {
    // The native enum argument uses 32-bit values; meanings of its values are unclaimed.
    enum PerCharacterTransformCenter : int;

    /* 0x00 */ const ShadowParameter* _0;
    /* 0x08 */ const PerCharacterTransformInfo* _8;
    /* 0x10 */ PerCharacterTransformCenter _10;
    /* 0x14 */ u8 _14[4];
    /* 0x18 */ const util::MatrixT4x3fType* _18;
    /* 0x20 */ const util::MatrixT4x3fType* _20;
    /* 0x28 */ util::Float4 _28;
    /* 0x38 */ util::Float4 _38;
    /* 0x48 */ u8 _48;
    /* 0x49 */ u8 _49[3];
    /* 0x4c */ util::Float4 _4c;
    /* 0x5c */ util::Float4 _5c;
    /* 0x6c */ u8 _6c;
    /* 0x6d */ u8 _6d[3];
    /* 0x70 */ u32 _70;
    /* 0x74 */ u8 _74[4];
};
static_assert(sizeof(ConstantBufferAdditionalContent) == 0x78);
static_assert(offsetof(ConstantBufferAdditionalContent, _18) == 0x18);
static_assert(offsetof(ConstantBufferAdditionalContent, _4c) == 0x4c);

// Partial layout: TextBox::AllocateStringBuffer allocates this 0xc0-byte object and extra GPU
// storage. Copying a TextBox reads the capacity at +0x98; per-character updates read +0x9c.
class DispStringBuffer {
public:
    // One appended glyph; CharWriter::PrintGlyph fills every field, BuildPerCharacterParams reads
    // the position, colors and the two flags.
    struct UnkA0 {
        /* 0x00 */ f32 width;
        /* 0x04 */ f32 height;
        /* 0x08 */ f32 x;
        /* 0x0c */ f32 y;
        /* 0x10 */ util::Unorm8x4 colorTop;
        /* 0x14 */ util::Unorm8x4 colorBottom;
        /* 0x18 */ f32 u0;
        /* 0x1c */ f32 v0;
        /* 0x20 */ f32 u1;
        /* 0x24 */ f32 v1;
        // TextureObject pointer; bit 0 is the glyph's border-effect flag.
        /* 0x28 */ u64 texture;
        // Per-character x offset, in font pixels.
        /* 0x30 */ s16 _30;
        /* 0x32 */ u8 _32;
        /* 0x33 */ u8 alpha;
        /* 0x34 */ u8 _34[4];
    };
    struct InitializeArg {
        UnkA0* _0 = nullptr;
        GpuBuffer* _8 = nullptr;
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

    // BuildConstantBuffer reserves 128 bytes, and the common-data writer fills through +0x7c.
    struct ShaderParam {
        util::MatrixT4x4fType matrix;
        util::Float4 _40;
        util::Float4 _50;
        util::Float4 _60;
        util::Float4 _70;
    };
    static_assert(sizeof(ShaderParam) == 0x80);

    void BuildCommonConstantBufferData(ShaderParam&, const util::MatrixT4x4fType&,
                                       const ConstantBufferAdditionalContent&) const;

    // BuildConstantBuffer and the drawing helper keep byte offsets at 0/4, flags at8,
    // and up to eight texture/count/flag records at10. Names are inferred from those uses.
    struct TextureEntry {
        const TextureObject* mTexture = nullptr;
        u32 mCount = 0;
        u32 mFlags = 0;
    };
    static_assert(sizeof(TextureEntry) == 0x10);

    // Draw and AddDrawCommand consume the texture groups and their count as a record.
    struct VertexBufferData {
        TextureEntry mTextures[8]{};
        s32 mTextureCount = 0;
        u8 _84[4]{};
    };
    static_assert(sizeof(VertexBufferData) == 0x88);

    /* 0x00 */ u32 mConstantBufferOffset = 0;
    /* 0x04 */ u32 mVertexBufferOffset = 0;
    /* 0x08 */ u32 mFlags = 0;
    /* 0x10 */ VertexBufferData mVertexBufferData{};
    /* 0x98 */ s32 mCapacity = 0;
    /* 0x9c */ s32 mCharCount = 0;
    /* 0xa0 */ UnkA0* _a0 = nullptr;
    /* 0xa8 */ u8* _a8 = nullptr;
    /* 0xb0 */ GpuBuffer* _b0 = nullptr;
    /* 0xb8 */ bool _b8 = false;
    /* 0xb9 */ bool _b9 = false;
    /* 0xba */ bool _ba = false;
    /* 0xbc */ f32 mFontHeight;
};
static_assert(sizeof(DispStringBuffer::InitializeArg) == 0x18);
static_assert(sizeof(DispStringBuffer::UnkA0) == 0x38);
static_assert(sizeof(DispStringBuffer) == 0xc0);

}  // namespace nn::font
