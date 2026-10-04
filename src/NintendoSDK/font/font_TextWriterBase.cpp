#include <nn/font/font_TextWriterBase.h>
#include <nn/font/font_TagProcessorBase.h>
#include <nn/font/font_PrintContext.h>

#include <limits>
#include <algorithm>

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

// 0x7101328578 (char) / 0x71013298f0 (u16)
// NON_MATCHING: the compiler loads and stores cursor scalars separately.
template <typename CharType>
f32 TextWriterBase<CharType>::Print(const CharType* string, s32 length) {
    TextWriterBase writer(*this);
    const f32 width = writer.PrintImpl(string, length, 0, nullptr, nullptr);
    mCursorX = writer.mCursorX;
    mCursorY = writer.mCursorY;
    return width;
}

// 0x71013285f8 (char) / 0x7101329970 (u16)
// NON_MATCHING: the compiler loads and stores cursor scalars separately.
template <typename CharType>
f32 TextWriterBase<CharType>::Print(const CharType* string, s32 length, s32 arg3,
                                  const f32* arg4, const f32* arg5) {
    TextWriterBase writer(*this);
    const f32 width = writer.PrintImpl(string, length, arg3, arg4, arg5);
    mCursorX = writer.mCursorX;
    mCursorY = writer.mCursorY;
    return width;
}

// 0x7101329190
// NON_MATCHING: null-string branch and line-bound update scheduling.
template <>
void TextWriterBase<u16>::CalculateStringRect(Rectangle* rect, const u16* string, s32 length) const {
    if (!string) {
        *rect = {};
        return;
    }

    TextWriterBase writer(*this);
    const u16* end = string + length;
    *rect = {};
    writer.SetCursorX(0.0f);
    writer.SetCursorY(0.0f);
    do {
        Rectangle line{};
        writer.CalculateLineRectImpl(&line, &string, length);
        rect->left = std::min(rect->left, line.left);
        rect->top = std::min(rect->top, line.top);
        rect->right = std::max(rect->right, line.right);
        rect->bottom = std::max(rect->bottom, line.bottom);
        length = end - string;
    } while (length > 0);
}

// 0x7101329150
template <>
f32 TextWriterBase<u16>::CalculateStringWidth(const u16* string, s32 length) const {
    Rectangle rect{};
    CalculateStringRect(&rect, string, length);
    return rect.right - rect.left;
}

// 0x71013292b8
template <>
const u16* TextWriterBase<u16>::FindPosOfWidthLimit(const u16* string, s32 length) const {
    Rectangle rect{};
    TextWriterBase writer(*this);
    writer.SetCursorX(0.0f);
    writer.SetCursorY(0.0f);
    writer.CalculateLineRectImpl(&rect, &string, length);
    return string;
}

template class TextWriterBase<char>;
template class TextWriterBase<u16>;

}  // namespace nn::font
