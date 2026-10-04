/**
 * @file font_TextureObject.h
 * @brief Glyph sheet texture of a font.
 */

#pragma once

#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_ResTexture.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>

namespace nn::font {

// Layout evidence: TextureObject::Set (0x7100ab3540) and ~TextureObject (0x7100ab34a0). ResFontBase embeds a
// ResourceTextureObject at +0x28 (its sheet format / black-white flag are read through that member).
class TextureObject {
public:
    TextureObject() { Reset(); }
    virtual ~TextureObject() { Reset(); }

    virtual const gfx::TextureView* GetTextureView() const = 0;
    virtual gfx::TextureView* GetTextureView() = 0;

    void Set(const void* data, u16 format, u16 width, u16 height, u8 sheet_count,
             bool black_white_interpolation);

    // Whether colors are interpolated between black and white (Font::IsColorBlackWhiteInterpolationEnabled reads it).
    bool IsColorBlackWhiteInterpolationEnabled() const { return mBlackWhiteInterpolation; }
    void SetColorBlackWhiteInterpolationEnabled(bool enabled) { mBlackWhiteInterpolation = enabled; }

    // inline-only in the original; names are guesses. Set / Reset and
    // ResFontBase::GenTextureNames / UnloadTexture establish these data and state reads.
    bool IsSet() const { return _18; }
    const void* GetData() const { return mData; }
    u16 GetFormat() const { return mFormat; }
    gfx::DescriptorSlot* GetDescriptorSlot() { return &mDescriptorSlot; }

protected:
    // inline-only in the original (constructor, destructor and ResourceTextureObject's constructor all run it); name is
    // a guess
    void Reset() {
        Set(nullptr, 0xc, 0, 0, 0, true);
        mDescriptorSlot.Invalidate();
        _18 = false;
    }

    /* 0x08 */ const void* mData;
    /* 0x10 */ u16 mHeight;
    /* 0x12 */ u16 mWidth;
    /* 0x14 */ u16 mFormat;
    /* 0x16 */ u8 mSheetCount;
    /* 0x17 */ bool mBlackWhiteInterpolation;
    /* 0x18 */ bool _18;
    /* 0x20 */ gfx::DescriptorSlot mDescriptorSlot;
};
static_assert(sizeof(TextureObject) == 0x28);

// A texture object over a texture resource (the font's glyph sheets).
class ResourceTextureObject : public TextureObject {
public:
    ResourceTextureObject() { Reset(); }

    void Initialize(gfx::Device* device, gfx::MemoryPool* pool, s64 pool_offset, u64 pool_size);

    const gfx::TextureView* GetTextureView() const override { return mResTexture->GetTextureView(); }
    gfx::TextureView* GetTextureView() override { return mResTexture->GetTextureView(); }

private:
    /* 0x28 */ gfx::ResTexture* mResTexture;
};
static_assert(sizeof(ResourceTextureObject) == 0x30);

}  // namespace nn::font
