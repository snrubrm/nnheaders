/**
 * @file font_Font.h
 * @brief Font interface (base of the resource and scalable fonts).
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>

namespace nn::font {

// Widths of one character, in this order and size: the three bytes are packed into a register result
// (ResFontBase::GetDefaultCharWidths / GetCharWidths).
struct CharWidths {
    s8 left;
    u8 glyphWidth;
    u8 charWidth;
};

class TextureObject;

// A glyph as handed to the writer (ResFontBase::GetGlyphFromIndex fills it; offsets from there). Names without a
// stated source follow the NintendoWare font library.
struct Glyph {
    const void* pTexture;  // the glyph sheet
    s16 leftWidth;
    u16 glyphWidth;
    u16 charWidth;
    u16 _e;  // the glyph width again
    u16 height;
    u16 _12;  // the cell height again
    u16 texWidth;
    u16 texHeight;
    u16 cellX;
    u16 cellY;
    u16 texFormat;
    u8 sheetIndex;
    u8 _1e;
    const TextureObject* pTextureObject;
    u16 _28;
};
static_assert(sizeof(Glyph) == 0x30);

// The vtable of the original (ResFontBase 0x252c568, ScalableFont 0x252c6e8): GetRuntimeTypeInfo, destructor (D1 / D0),
// Finalize, then the pure virtuals below in this order. The names follow the NintendoWare font library.
class Font {
public:
    // Values of GetType (ResFontBase returns 1, ScalableFont 2).
    enum Type {
        Type_Invalid,
        Type_Resource,
        Type_Scalable,
    };

    // Texture formats of the glyph sheets (ScalableFont returns 8, ResFontBase the format of its sheets).
    enum TextureFormat {
        TextureFormat_A8 = 8,
    };

    NN_RUNTIME_TYPEINFO_BASE()

    Font();
    virtual ~Font();

    virtual void Finalize(gfx::Device*);

    virtual s32 GetWidth() const = 0;
    virtual s32 GetHeight() const = 0;
    virtual s32 GetAscent() const = 0;
    virtual s32 GetDescent() const = 0;
    virtual s32 GetMaxCharWidth() const = 0;
    virtual Type GetType() const = 0;
    virtual u32 GetTextureFormat() const = 0;
    virtual s32 GetLineFeed() const = 0;
    virtual CharWidths GetDefaultCharWidths() const = 0;
    virtual void SetLineFeed(s32 line_feed) = 0;
    virtual void SetDefaultCharWidths(const CharWidths& widths) = 0;
    virtual bool SetAlternateChar(u32 code) = 0;
    virtual s32 GetCharWidth(u32 code) const = 0;
    virtual CharWidths GetCharWidths(u32 code) const = 0;
    virtual void GetGlyph(Glyph* glyph, u32 code) const = 0;
    virtual bool HasGlyph(u32 code) const = 0;
    virtual s32 GetKerning(u32 first, u32 second) const = 0;
    virtual u32 GetCharacterCode() const = 0;
    virtual s32 GetBaselinePos() const = 0;
    virtual s32 GetCellHeight() const = 0;
    virtual s32 GetCellWidth() const = 0;
    virtual void SetLinearFilterEnabled(bool at_small, bool at_large) = 0;
    virtual bool IsLinearFilterEnabledAtSmall() const = 0;
    virtual bool IsLinearFilterEnabledAtLarge() const = 0;
    virtual u32 GetTextureWrapFilterValue() const = 0;
    virtual bool IsColorBlackWhiteInterpolationEnabled() const = 0;
    virtual void SetColorBlackWhiteInterpolationEnabled(bool enabled) = 0;
    virtual bool IsBorderEffectEnabled() const = 0;

protected:
    // Both set by the constructor (one halfword store of 0x0101). GetKerning of both font classes returns 0 while
    // `mKerningEnabled` is off.
    bool mKerningEnabled;
    bool _9;
};
static_assert(sizeof(Font) == 0x10);

}  // namespace nn::font
