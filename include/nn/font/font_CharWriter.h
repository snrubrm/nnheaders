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
    f32 PrintGlyph(const Glyph& glyph);

    void SetScale(f32 x, f32 y) {
        mScaleX = x;
        mScaleY = y;
    }

    // inline-only in the original; names are guesses. Scale reads recur in
    // GetFontWidth / GetFontHeight and eui::TagProcessor (0x7100be6fa0).
    f32 GetScaleX() const { return mScaleX; }
    f32 GetScaleY() const { return mScaleY; }

    // inline-only in the original; names are guesses. Reads recur in
    // TagProcessor::processPictFontProcessTag_ (0x7100be6dcc) and
    // CharWriter::PrintGlyph (0x7101323dc8); stores recur in the same
    // tag handler and TextBox::SetupTextWriter (0x7100abc3d0).
    f32 GetItalicRatio() const { return mItalicRatio; }
    void SetItalicRatio(f32 ratio) { mItalicRatio = ratio; }

    f32 GetCursorX() const { return mCursorX; }
    f32 GetCursorY() const { return mCursorY; }
    void SetCursorX(f32 x) { mCursorX = x; }
    void SetCursorY(f32 y) { mCursorY = y; }

    // inline-only in the original; name is a guess. The color reads recur in
    // eui::TagProcessor::BeginPrint (0x7100be5c70) and glyph drawing (0x7101323dc8).
    const util::Unorm8x4& GetTextColor(s32 index) const { return mTextColors[index]; }

    // inline-only in the original; names are guesses. The two color stores recur in
    // TextBox::SetupTextWriter (0x7100abc3d0) and eui::TagProcessor (0x7100be6c84 / be6d58).
    void SetTextColor(const util::Unorm8x4& top, const util::Unorm8x4& bottom) {
        mTextColors[0] = top;
        mTextColors[1] = bottom;
    }

    // inline-only in the original; name is a guess. The font store recurs in
    // TextBox::SetupTextWriter (0x7100abc3d0) and eui::TagProcessor (0x7100be6bec).
    void SetFont(const Font* font) { mFont = font; }
    // inline-only in the original; name is a guess. CharWriter's constructor and
    // eui::TagProcessor (0x7100be6d58) write the byte at 0x39.
    void SetAlpha(u8 alpha) { mAlpha = alpha; }

    const Font* GetFont() const { return mFont; }
    bool IsWidthFixed() const { return mIsWidthFixed; }
    f32 GetFixedWidth() const { return mFixedWidth; }

private:
    void sub_7101323DC8(const Glyph* glyph, f32 glyph_x);

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
