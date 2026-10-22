#include <nn/font/font_CharWriter.h>
#include <nn/font/font_DispStringBuffer.h>

namespace nn::font {

// NON_MATCHING: same stores; the original writes the two text colors as separate 32 bit stores (a pair store with the
// next word) and orders the zero stores differently
// 0x7101323bf0
CharWriter::CharWriter()
    : mTextColors{{{0xff, 0xff, 0xff, 0xff}}, {{0xff, 0xff, 0xff, 0xff}}}, _8(0), mScaleX(1.0f), mScaleY(1.0f),
      mCursorX(0.0f), mCursorY(0.0f), mCursorZ(0.0f), mFixedWidth(0.0f), mItalicRatio(0.0f), mFont(nullptr),
      mDispStringBuffer(nullptr), mIsWidthFixed(false), mAlpha(0xff) {}

// 0x7101323c24
CharWriter::~CharWriter() {}

// 0x7101323fdc: drops the glyphs accumulated in the writer's DispStringBuffer.
void CharWriter::StartPrint() {
    mDispStringBuffer->mCharCount = 0;
}

// 0x7101323fe8
void CharWriter::EndPrint() const {}

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

// 0x7101323d30
f32 CharWriter::PrintGlyph(const Glyph& glyph) {
    f32 advance;
    f32 left;
    if (mIsWidthFixed) {
        advance = mFixedWidth;
        left = (advance - glyph.charWidth * mScaleX) * 0.5f + mScaleX * glyph.leftWidth;
    } else {
        advance = glyph.charWidth * mScaleX;
        left = mScaleX * glyph.leftWidth;
    }
    PrintGlyph(mCursorX + left, glyph);
    mCursorX += advance;
    return advance;
}

// NON_MATCHING: same instructions and size (532 B); the remaining differences are the scheduler's
// order in the two coordinate branches and register allocation (v0/v1/y land in different s
// registers), plus an equivalent inverted fcsel and orr operand order.
// 0x7101323dc8
void CharWriter::PrintGlyph(f32 x, const Glyph& glyph) {
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    f32 width;
    f32 height;
    f32 y;
    if (mFont->_9) {
        u0 = (f32(glyph.cellX) - 0.5f) / f32(glyph.texWidth);
        u1 = (f32(glyph.cellX + glyph._e) + 0.5f) / f32(glyph.texWidth);
        v0 = (f32(glyph.cellY) - 0.5f) / f32(glyph.texHeight);
        v1 = (f32(glyph.cellY + glyph._12) + 0.5f) / f32(glyph.texHeight);
        width = (f32(glyph.glyphWidth) + 1.0f) * mScaleX;
        height = (f32(glyph.height) + 1.0f) * mScaleY;
        x -= mScaleX * 0.5f;
        y = mCursorY - mScaleY * 0.5f;
    } else {
        u0 = f32(glyph.cellX) / f32(glyph.texWidth);
        u1 = f32(glyph.cellX + glyph._e) / f32(glyph.texWidth);
        v0 = f32(glyph.cellY) / f32(glyph.texHeight);
        v1 = f32(glyph.cellY + glyph._12) / f32(glyph.texHeight);
        width = f32(glyph.glyphWidth) * mScaleX;
        height = f32(glyph.height) * mScaleY;
        y = mCursorY;
    }

    if (mDispStringBuffer->mCharCount < mDispStringBuffer->mCapacity) {
        const s32 index = mDispStringBuffer->mCharCount;
        mDispStringBuffer->mCharCount = index + 1;
        DispStringBuffer::UnkA0* record = &mDispStringBuffer->_a0[index];
        record->width = width;
        record->height = height;
        record->x = x;
        record->y = y;
        record->colorTop = mTextColors[0];
        record->colorBottom = mTextColors[1];
        record->u0 = u0;
        record->v0 = 1.0f - v0;
        record->u1 = u1;
        record->v1 = 1.0f - v1;
        record->alpha = mAlpha;
        record->_30 = s16(mItalicRatio * mScaleX * f32(mFont->GetWidth()));
        const f32 scaleY = mScaleY;
        f32 fontHeight = scaleY * f32(mFont->GetHeight());
        fontHeight = mFont->_9 ? fontHeight + scaleY : fontHeight;
        mDispStringBuffer->SetFontHeight(fontHeight);
        record->_32 = glyph._1e;
        record->texture = (mFont->IsBorderEffectEnabled() & 1) | glyph.textureObject;
    }
}

}  // namespace nn::font
