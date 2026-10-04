#pragma once

#include <nn/types.h>

namespace nn::font {

// Only the static line-size classification API is recovered; no node layout
// or allocation uses this partial declaration.
struct GlyphNode {
    static u8 CalculateLineKind(u16 height);
};

}  // namespace nn::font
