/**
 * @file Material.h
 * @brief UI Material implementation.
 */

#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>
#include <nn/ui2d/Types.h>

namespace nn {
namespace ui2d {
class AnimTransform;
class BuildResultInformation;
struct BuildArgSet;
struct ResMaterial;
struct UserShaderInformation;

class TextureInfo;

// One texture slot of a material: byte 0 holds the sampler settings (0x90 once a texture info has been set: clamp,
// linear / linear), the texture info pointer is at +8 (TexMap::TexMap / TexMap::Set 0x7100aba554 / 0x7100aba568
// store both).
class TexMap {
public:
    TexMap();
    explicit TexMap(const TextureInfo* info);
    ~TexMap();
    void Finalize();
    void Set(const TextureInfo* info);
    void SetWrapMode(TexWrap wrap_s, TexWrap wrap_t);
    void SetFilter(TexFilter min_filter, TexFilter mag_filter);

    const TextureInfo* GetTextureInfo() const { return mTextureInfo; }
    // Name is a guess: the callers (eui::ApplyTextureInfoToMaterial, texture unloading) replace only the pointer.
    void ReplaceTextureInfo(const TextureInfo* info) { mTextureInfo = info; }

private:
    union {
        u8 mFlags;
        struct {
            u8 mWrapS : 2;
            u8 mWrapT : 2;
            u8 mMinFilter : 3;
            u8 mMagFilter : 1;
        };
    };
    const TextureInfo* mTextureInfo;
};
static_assert(sizeof(TexMap) == 0x10);

class Material {
public:
    Material();
    Material(const Material&, gfx::Device*);
    Material(BuildResultInformation*, gfx::Device*, const ResMaterial*, const ResMaterial*,
             const BuildArgSet&);

    void Initialize();
    void Finalize(gfx::Device*);
    void ReserveMem(s32, s32, s32, s32, bool, s32, bool, s32, bool, bool);
    void SetupUserShaderConstantBufferInformation(nn::ui2d::UserShaderInformation const&);

    virtual ~Material();
    virtual void BindAnimation(nn::ui2d::AnimTransform*);
    virtual void UnbindAnimation(nn::ui2d::AnimTransform*);

    // Names are guesses. Evidence: Material::ReserveMem (0x7100ac27c4) and Material(const Material&) (0x7100ac3398)
    // read / write the two capacity words, Picture::Append (0x7100ab9e8c) and eui::ApplyTextureInfoToMaterial
    // (0x7100bed6bc) read the count and index the array.
    // Inline-only in the original; name follows Pane/Group. Picture::Finalize and
    // TextBox::Finalize skip destruction when bit 0 at +0x4a is set.
    bool IsUserAllocated() const { return detail::TestBit(mFlags, 0); }

    s32 GetTexMapCount() const { return mMemCount.texMap; }
    TexMap* GetTexMapArray() const { return static_cast<TexMap*>(mMem); }

    // inline-only in the original; name is a guess. The bounded comparison
    // repeats in both Pane material searches and BindMaterial (0x7100ab4a50).
    bool IsNameEqual(const char* name) const {
        for (s32 i = 0; i < 28; ++i) {
            if (mName[i] != name[i])
                return false;
            if (mName[i] == '\0')
                return true;
        }
        return true;
    }

private:
    // Allocation counts (the same bit layout is used for the capacity at +0x10 and the used counts at +0x14; the
    // order is the order of ReserveMem's parameters). Bit 16 of the capacity word is also cleared by ReserveMem.
    struct MemInfo {
        u32 texMap : 2;
        u32 texSrt : 2;
        u32 texCoordGen : 2;
        u32 tevStage : 3;
        u32 alphaCompare : 1;
        u32 blend : 2;
        u32 indirect : 1;
        u32 projTexGen : 2;
        u32 fontShadow : 1;
        u32 _bit16 : 1;
        u32 _padding : 15;
    };

    /* 0x08 */ u8 _8[0x8];
    /* 0x10 */ MemInfo mMemCap;
    /* 0x14 */ MemInfo mMemCount;
    /* 0x18 */ void* mMem;  // one allocation; starts with the TexMap array
    /* 0x20 */ u8 _20[0x8];
    /* 0x28 */ const char* mName;  // copyCtor 0x7100ac33f4; Pane searches dereference it
    /* 0x30 */ u8 _30[0x8];
    /* 0x38 */ void* _38;
    /* 0x40 */ void* _40;
    /* 0x48 */ u8 _48[2];
    /* 0x4a */ u8 mFlags;
    /* 0x4b */ u8 _4b;
};
}  // namespace ui2d
}  // namespace nn
