/**
 * @file font_TextWriterBase.h
 * @brief Writes strings of CharType characters with a TagProcessorBase.
 */

#pragma once

#include <nn/font/font_CharWriter.h>

namespace nn::font {

template <typename CharType>
class TagProcessorBase;
class Rectangle;

// Instantiated for char and unsigned short. Members past CharWriter: TextWriterBase::TextWriterBase (0x71013290b8),
// GetLineHeight (0x7101329104), TextBox::SetupTextWriter, the tab / line processing in TagProcessorBase.
template <typename CharType>
class TextWriterBase : public CharWriter {
public:
    TextWriterBase();
    ~TextWriterBase();

    f32 GetLineHeight() const;
    s32 GetTabWidth() const { return mTabWidth; }
    f32 CalculateStringWidth(const CharType* string, s32 length) const;
    void CalculateStringRect(Rectangle*, const CharType* string, s32 length) const;

    // inline-only in the original; name is a guess (TextBox::SetupTextWriter and
    // TextBoxEx::adjustText_ both set this width limit).
    void SetWidthLimit(f32 limit) { mWidthLimit = limit; }

protected:
    static TagProcessorBase<CharType> sDefaultTagProcessor;

    /* 0x3c */ f32 mWidthLimit;
    /* 0x40 */ f32 mCharSpace;
    /* 0x44 */ f32 mLineSpace;
    /* 0x48 */ u32 _48;
    /* 0x4c */ s32 mTabWidth;
    /* 0x50 */ u32 mDrawFlag;
    /* 0x58 */ TagProcessorBase<CharType>* mTagProcessor;
    /* 0x60 */ u16 _60;
};

}  // namespace nn::font
