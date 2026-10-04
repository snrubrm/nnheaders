#include <nn/font/font_CharWriter.h>

namespace nn::font {

// NON_MATCHING: same stores; the original writes the two text colors as separate 32 bit stores (a pair store with the
// next word) and orders the zero stores differently
// 0x7101323bf0
CharWriter::CharWriter()
    : mTextColors{{{0xff, 0xff, 0xff, 0xff}}, {{0xff, 0xff, 0xff, 0xff}}}, _8(0), mScaleX(1.0f), mScaleY(1.0f),
      mCursorX(0.0f), mCursorY(0.0f), mCursorZ(0.0f), mFixedWidth(0.0f), mItalicRatio(0.0f), mFont(nullptr),
      _30(nullptr), mIsWidthFixed(false), mAlpha(0xff) {}

// 0x7101323c24
CharWriter::~CharWriter() {}

// 0x7101323c88
f32 CharWriter::GetFontWidth() const {
    return mFont->GetWidth() * mScaleX;
}

// 0x7101323cc0
f32 CharWriter::GetFontHeight() const {
    return mFont->GetHeight() * mScaleY;
}

// 0x7101323cf8
f32 CharWriter::GetFontAscent() const {
    return mFont->GetAscent() * mScaleY;
}

// 0x7101323c28
void CharWriter::SetFontSize(f32 width, f32 height) {
    SetScale(width / mFont->GetWidth(), height / mFont->GetHeight());
}

}  // namespace nn::font
