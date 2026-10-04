#include <nn/font/font_TextWriterBase.h>
#include <nn/font/font_TagProcessorBase.h>
#include <nn/font/font_PrintContext.h>

#include <limits>

namespace nn::font {

template <typename CharType>
TagProcessorBase<CharType> TextWriterBase<CharType>::sDefaultTagProcessor;

template <typename CharType>
TextWriterBase<CharType>::TextWriterBase()
    : mWidthLimit(std::numeric_limits<f32>::max()), mCharSpace(0.0f), mLineSpace(0.0f), _48(0),
      mTabWidth(4), mDrawFlag(0), mTagProcessor(&sDefaultTagProcessor), _60(0) {}

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

// 0x7101329150
template <>
f32 TextWriterBase<u16>::CalculateStringWidth(const u16* string, s32 length) const {
    Rectangle rect{};
    CalculateStringRect(&rect, string, length);
    return rect.right - rect.left;
}

template class TextWriterBase<char>;
template class TextWriterBase<u16>;

}  // namespace nn::font
