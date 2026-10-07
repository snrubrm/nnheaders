/**
 * @file font_ScalableFont.h
 * @brief Font that renders outline glyphs into a texture cache.
 */

#pragma once

#include <nn/font/font_Font.h>

namespace nn::font {

class TextureCache;

class ScalableFont : public Font {
public:
    NN_RUNTIME_TYPEINFO(Font)

    struct InitializeArg {
        void SetDefault();

        /* 0x00 */ TextureCache* pTextureCache;
        /* 0x08 */ s32 fontSize;  // 40
        /* 0x0c */ u16 _c;
        /* 0x0e */ u16 _e;
        /* 0x10 */ u32 alternateChar;  // '?'
    };

    ScalableFont();
    ~ScalableFont() override;

    s32 GetWidth() const override;
    s32 GetHeight() const override;
    s32 GetAscent() const override;
    s32 GetDescent() const override;
    s32 GetMaxCharWidth() const override;
    Type GetType() const override;
    u32 GetTextureFormat() const override;
    s32 GetLineFeed() const override;
    CharWidths GetDefaultCharWidths() const override;
    void SetLineFeed(s32 line_feed) override;
    void SetDefaultCharWidths(const CharWidths& widths) override;
    bool SetAlternateChar(u32 code) override;
    s32 GetCharWidth(u32 code) const override;
    CharWidths GetCharWidths(u32 code) const override;
    void GetGlyph(Glyph* glyph, u32 code) const override;
    bool HasGlyph(u32 code) const override;
    s32 GetKerning(u32 first, u32 second) const override;
    u32 GetCharacterCode() const override;
    s32 GetBaselinePos() const override;
    s32 GetCellHeight() const override;
    s32 GetCellWidth() const override;
    void SetLinearFilterEnabled(bool at_small, bool at_large) override;
    bool IsLinearFilterEnabledAtSmall() const override;
    bool IsLinearFilterEnabledAtLarge() const override;
    u32 GetTextureWrapFilterValue() const override;
    bool IsColorBlackWhiteInterpolationEnabled() const override;
    void SetColorBlackWhiteInterpolationEnabled(bool enabled) override;
    bool IsBorderEffectEnabled() const override;
    void RegisterAlternateCharGlyph() const;

private:
    /* 0x10 */ TextureCache* mTextureCache;
    /* 0x18 */ s32 mWidth;
    /* 0x1c */ s32 mHeight;
    /* 0x20 */ s32 mAscent;
    /* 0x24 */ s32 mBaselinePos;
    /* 0x28 */ s32 mLineFeed;
    /* 0x2c */ CharWidths mDefaultCharWidths;
    /* 0x2f */ u8 _2f;
    /* 0x30 */ u16 mFontFace;
    /* 0x34 */ u32 mAlternateChar;
};
static_assert(sizeof(ScalableFont) == 0x38);

}  // namespace nn::font
