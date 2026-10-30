#pragma once

#include <nn/types.h>

namespace nn::font {

// Tree links and cache flags are read by GlyphTreeMap. ScalableFont's glyph and width queries
// establish the request mask and three width fields; the list and remaining payload are unknown.
struct GlyphNode {
    static u8 CalculateLineKind(u16 height);

    u8 _0[0x20];
    GlyphNode* left;
    GlyphNode* right;
    u64 key;
    u8 flags;
    u8 _39[3];
    u32 requestMask;
    u8 _40[8];
    u16 glyphWidth;
    u8 _4a[2];
    u16 charWidth;
    s16 leftWidth;
    u8 _50[8];
};
static_assert(sizeof(GlyphNode) == 0x58);

}  // namespace nn::font
