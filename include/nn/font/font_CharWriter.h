/**
 * @file font_CharWriter.h
 * @brief Writes single glyphs (cursor, scale, colors).
 */

#pragma once

#include <nn/font/font_Font.h>
#include <nn/util/MathTypes.h>

namespace nn::font {

// Layout evidence: CharWriter::CharWriter (0x7101323bf0), SetFontSize / GetFontWidth / GetFontHeight /
// GetFontAscent (0x7101323c28 ..) and TextBox::SetupTextWriter. Names follow the NintendoWare font library.
class CharWriter {
public:
    CharWriter();
    ~CharWriter();

    void SetFontSize(f32 width, f32 height);
    f32 GetFontWidth() const;
    f32 GetFontHeight() const;
    f32 GetFontAscent() const;
    void PrintGlyph(const Glyph& glyph);

    void SetScale(f32 x, f32 y) {
        mScaleX = x;
        mScaleY = y;
    }

    f32 GetCursorX() const { return mCursorX; }
    f32 GetCursorY() const { return mCursorY; }
    void SetCursorX(f32 x) { mCursorX = x; }
    void SetCursorY(f32 y) { mCursorY = y; }

    const Font* GetFont() const { return mFont; }
    bool IsWidthFixed() const { return mIsWidthFixed; }
    f32 GetFixedWidth() const { return mFixedWidth; }

protected:
    /* 0x00 */ util::Unorm8x4 mTextColors[2];  // top, bottom
    /* 0x08 */ u32 _8;
    /* 0x0c */ f32 mScaleX;
    /* 0x10 */ f32 mScaleY;
    /* 0x14 */ f32 mCursorX;
    /* 0x18 */ f32 mCursorY;
    /* 0x1c */ f32 mCursorZ;
    /* 0x20 */ f32 mFixedWidth;
    /* 0x24 */ f32 mItalicRatio;
    /* 0x28 */ const Font* mFont;
    /* 0x30 */ void* _30;
    /* 0x38 */ bool mIsWidthFixed;
    /* 0x39 */ u8 mAlpha;
};

}  // namespace nn::font
