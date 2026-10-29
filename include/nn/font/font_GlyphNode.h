#pragma once

#include <nn/types.h>

namespace nn::font {

// Tree links and cache flags are read by GlyphTreeMap. The list and glyph payload fields
// remain unmodelled; Initialize allocates nodes at a stride of 0x58.
struct GlyphNode {
    static u8 CalculateLineKind(u16 height);

    u8 _0[0x20];
    GlyphNode* left;
    GlyphNode* right;
    u64 key;
    u8 flags;
    u8 _39[0x58 - 0x39];
};
static_assert(sizeof(GlyphNode) == 0x58);

}  // namespace nn::font
