#include <nn/font/font_ScalableFont.h>

#include <nn/font/font_TextureCache.h>

namespace nn::font {

// NON_MATCHING: same stores; ours merges the two halfword zeros with the size into a pair store (the original keeps
// them as separate halfword stores)
// 0x71013270f0
void ScalableFont::InitializeArg::SetDefault() {
    pTextureCache = nullptr;
    fontSize = 40;
    _c = 0;
    _e = 0;
    alternateChar = '?';
}

// 0x7101327328
s32 ScalableFont::GetWidth() const {
    return mWidth;
}

// 0x7101327330
s32 ScalableFont::GetHeight() const {
    return mHeight;
}

// 0x7101327338
s32 ScalableFont::GetAscent() const {
    return mAscent;
}

// 0x7101327340
s32 ScalableFont::GetDescent() const {
    return mHeight - mAscent;
}

// 0x710132734c
s32 ScalableFont::GetMaxCharWidth() const {
    return mWidth;
}

// 0x7101327354
Font::Type ScalableFont::GetType() const {
    return Type_Scalable;
}

// 0x710132735c
u32 ScalableFont::GetTextureFormat() const {
    return TextureFormat_A8;
}

// 0x7101327364
s32 ScalableFont::GetLineFeed() const {
    return mLineFeed;
}

// 0x710132736c
CharWidths ScalableFont::GetDefaultCharWidths() const {
    return mDefaultCharWidths;
}

// 0x710132737c
void ScalableFont::SetLineFeed(s32 line_feed) {
    mLineFeed = line_feed;
}

// 0x7101327384
void ScalableFont::SetDefaultCharWidths(const CharWidths& widths) {
    mDefaultCharWidths = widths;
}

// 0x7101327398
bool ScalableFont::SetAlternateChar(u32 code) {
    if (_2f != 0 && mTextureCache->IsGlyphExistInFont(code, mFontFace)) {
        mAlternateChar = code;
        return true;
    }
    return false;
}

// 0x710132771c
s32 ScalableFont::GetKerning(u32 first, u32 second) const {
    if (mKerningEnabled)
        return mTextureCache->CalculateKerning(first, second, mWidth, mFontFace);
    return 0;
}

// 0x7101327740
u32 ScalableFont::GetCharacterCode() const {
    return 1;
}

// 0x7101327748
s32 ScalableFont::GetBaselinePos() const {
    return mBaselinePos;
}

// 0x7101327750
s32 ScalableFont::GetCellHeight() const {
    return mHeight;
}

// 0x7101327758
s32 ScalableFont::GetCellWidth() const {
    return mWidth;
}

// 0x7101327760
void ScalableFont::SetLinearFilterEnabled(bool, bool) {}

// 0x7101327764
bool ScalableFont::IsLinearFilterEnabledAtSmall() const {
    return true;
}

// 0x710132776c
bool ScalableFont::IsLinearFilterEnabledAtLarge() const {
    return true;
}

// 0x7101327774
u32 ScalableFont::GetTextureWrapFilterValue() const {
    return 0;
}

// 0x710132777c
bool ScalableFont::IsColorBlackWhiteInterpolationEnabled() const {
    return true;
}

// 0x7101327784
bool ScalableFont::IsBorderEffectEnabled() const {
    if (mTextureCache)
        return mTextureCache->IsBorderEffectEnabled(mFontFace);
    return false;
}

// 0x710132779c
void ScalableFont::SetColorBlackWhiteInterpolationEnabled(bool) {}

}  // namespace nn::font
