/**
 * @file ResMaterial.h
 * @brief Resource material for models.
 */

#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>
#include <nn/g3d/TextureRef.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace g3d {

class ResMaterial {
public:
    u64 BindTexture(nn::g3d::TextureRef (*)(char const*, void*), void*);
    void ForceBindTexture(nn::g3d::TextureRef const&, char const*);
    void ReleaseTexture();
    void Setup(gfx::Device*);
    void Cleanup(gfx::Device*);
    void Reset();
    void Reset(u32);

    // Inline-only in the original; names are ours. The tera shader binding functions
    // store 0x1705 at +8 and 0 at +0xc after FindMaterial (0x71011413b4 and siblings).
    void set_8(u32 value) { m_8 = value; }
    void set_c(u16 value) { m_c = value; }
    // gsys::G3dResMaterialEx::setResMaterial 0x7100c6d21c stores its address at +0x88.
    void set_88(void* value) { m_88 = value; }

    // Native BindTexture ac895c and AGL bindings b33f24/b33f50.
    u8 _0[0x8];
    u32 m_8;
    u16 m_c;
    u8 _e[0x10 - 0xe];

    // Inline-only in the original; name is a guess. gsys::ModelNW::getMaterialName
    // 0x7100c06f30 builds its SafeString from this pointer plus two characters.
    const char* m_pName;

    u8 _18[0x30 - 0x18];
    const gfx::TextureView** mTextureViews;  // 30
    nn::util::BinPtrToString* mTextureNames;  // 38
    u8 _40[0x48 - 0x40];
    // 0x20-byte per-texture records at +0x48. 0x7101140644 reads the half-texel value at +0x10,
    // 0x710114065c the float at +0x8, and terrain 0x71011473a0 caches +0x8.
    struct TextureRecord {
        u8 _0[8];
        f32 _8;
        u8 _c[4];
        f32 _10;
        u8 _14[0xc];
    };
    static_assert(sizeof(TextureRecord) == 0x20);
    TextureRecord* mTextureRecords;  // 48
    u8 _50[0x88 - 0x50];
    void* m_88;
    u8 _90[0x98 - 0x90];
    gfx::DescriptorSlot* mTextureDescriptorSlots;  // 98
    u8 _a0[8];
    u8 mTextureCount;  // a8
    // Graphics 0x7100f2e0f4 bounds agl::g3d::MaterialObj::GetResTexture's index with this byte.
    u8 _a9;
    u8 _aa[0xb4 - 0xaa];
};
}  // namespace g3d
}  // namespace nn
