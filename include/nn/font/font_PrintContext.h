/**
 * @file font_PrintContext.h
 * @brief State of a text print / calculation passed to the tag processor.
 */

#pragma once

#include <nn/types.h>

namespace nn::font {

template <typename CharType>
class TextWriterBase;

// Layout evidence: TagProcessorBase::Process / CalculateRect read the writer at +0 and the x origin at +0x18 (the
// middle members follow the NintendoWare font library).
template <typename CharType>
struct PrintContext {
    TextWriterBase<CharType>* writer;
    const CharType* str;
    const CharType* strEnd;
    f32 xOrigin;
    f32 yOrigin;
    u32 flags;
    void* userData;
};

// Rectangle of a text (CalculateRect writes the four edges and normalises them).
class Rectangle {
public:
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
