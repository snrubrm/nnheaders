/**
 * @file font_PrintContext.h
 * @brief State of a text print / calculation passed to the tag processor.
 */

#pragma once

#include <nn/types.h>

namespace nn::font {

template <typename CharType>
class TextWriterBase;

// Layout evidence: TextWriterBase<u16>::PrintImpl (0x7101329a04) constructs this
// 0x30-byte context at sp+0xb0 and copies CharWriter scaleX/scaleY to +0x20/+0x24.
// TagProcessorBase reads the writer at +0 and x origin at +0x18.
template <typename CharType>
struct PrintContext {
    TextWriterBase<CharType>* writer;
    const CharType* str;
    const CharType* strEnd;
    f32 xOrigin;
    f32 yOrigin;
    f32 scaleX;
    f32 scaleY;
    // PrintImpl and CalculateLineRectImpl pass flags and the previous code separately.
    u32 flags;
    u32 previousChar;
};

static_assert(sizeof(PrintContext<char>) == 0x30);
static_assert(sizeof(PrintContext<u16>) == 0x30);

// Rectangle of a text (CalculateRect writes the four edges and normalises them).
class Rectangle {
public:
    Rectangle() : left(0), top(0), right(0), bottom(0) {}
    Rectangle(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {}
    // User-provided: ui2d::Layout::GetLayoutRect 0x7100ab7970 returns the rectangle through x8
    // (indirect result), so the type is not trivially copyable.
    Rectangle(const Rectangle& other)
        : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}

    f32 left;
    f32 top;
    f32 right;
    f32 bottom;

    // Makes left <= right and top <= bottom.
    void Normalize() {
        const f32 l = left;
        const f32 t = top;
        const f32 r = right;
        const f32 b = bottom;
        const bool width_ok = r - l >= 0.0f;
        const bool height_ok = b - t >= 0.0f;
        left = width_ok ? l : r;
        top = height_ok ? t : b;
        right = width_ok ? r : l;
        bottom = height_ok ? b : t;
    }
};

}  // namespace nn::font
