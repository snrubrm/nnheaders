#include <nn/font/font_TextWriterBase.h>

namespace nn::font {

// 0x7101329100
template <typename CharType>
TextWriterBase<CharType>::~TextWriterBase() {}

// 0x7101329104
template <typename CharType>
f32 TextWriterBase<CharType>::GetLineHeight() const {
    f32 height = 0.0f;
    if (mFont)
        height = mFont->GetLineFeed();
    return height * mScaleY + mLineSpace;
}

template class TextWriterBase<char>;
template class TextWriterBase<u16>;

}  // namespace nn::font
