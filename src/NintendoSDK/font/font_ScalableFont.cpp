#include <nn/font/font_ScalableFont.h>

#include <nn/font/font_GlyphNode.h>
#include <nn/font/font_TextureCache.h>

namespace nn::font {

// 0x7101327110
// NON_MATCHING: member initialization store ordering and width-byte grouping.
ScalableFont::ScalableFont()
    : mTextureCache(nullptr), mWidth(40), mHeight(50), mAscent(40), mBaselinePos(40),
      mLineFeed(50), mDefaultCharWidths{0, 40, 40}, _2f(2), mFontFace(0),
      mAlternateChar('?') {}

// 0x7101327178 / 0x710132717c
ScalableFont::~ScalableFont() {}

// NON_MATCHING: same stores; ours merges the two halfword zeros with the size into a pair store (the original keeps
// them as separate halfword stores)
// 0x71013270f0
void ScalableFont::InitializeArg::SetDefault() {
    pTextureCache = nullptr;
    fontSize = 40;
    _c = 0;
    _e = 0;
    _f = 0;
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

// NON_MATCHING: the receiver and first argument loads are ordered differently.
void ScalableFont::RegisterAlternateCharGlyph() const {
    mTextureCache->RegisterGlyph(mAlternateChar, mWidth, mFontFace, -1);
    mTextureCache->FindGlyphNode(mAlternateChar, mWidth, mFontFace)->flags |= 0x20;
    if (_2f == 1) {
        mTextureCache->RegisterGlyph(' ', mWidth, mFontFace, -1);
        if (mTextureCache->FindGlyphNode(' ', mWidth, mFontFace))
            mTextureCache->FindGlyphNode(' ', mWidth, mFontFace)->flags |= 0x20;
    }
}

s32 ScalableFont::GetCharWidth(u32 code) const {
    GlyphNode* node = mTextureCache->FindGlyphNode(code, mWidth, mFontFace);
    if (!node || (node->flags & 0x42)) {
        if (_2f != 2 && mTextureCache->IsGlyphExistInFont(code, mFontFace))
            return mTextureCache->CalculateCharWidth(code, mWidth, mFontFace);
        node = mTextureCache->FindGlyphNode(mAlternateChar, mWidth, mFontFace);
    }
    return node->charWidth;
}

CharWidths ScalableFont::GetCharWidths(u32 code) const {
    GlyphNode* node = mTextureCache->FindGlyphNode(code, mWidth, mFontFace);
    if (!node || (node->flags & 0x42)) {
        if (_2f != 2 && mTextureCache->IsGlyphExistInFont(code, mFontFace)) {
            const s32 width = mTextureCache->CalculateCharWidth(code, mWidth, mFontFace);
            return {0, static_cast<u8>(width), static_cast<u8>(width)};
        }
        node = mTextureCache->FindGlyphNode(mAlternateChar, mWidth, mFontFace);
    }
    return {static_cast<s8>(node->leftWidth), static_cast<u8>(node->glyphWidth),
            static_cast<u8>(node->charWidth)};
}

bool ScalableFont::HasGlyph(u32 code) const {
    GlyphNode* node = mTextureCache->FindGlyphNode(code, mWidth, mFontFace);
    if (!node || (node->flags & 0x42))
        return false;
    if (node->flags & 0x35)
        return true;
    return node->requestMask != 0;
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
