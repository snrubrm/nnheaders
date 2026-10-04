#include <nn/font/font_CharWriter.h>

namespace nn::font {

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
