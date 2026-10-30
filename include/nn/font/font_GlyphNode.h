#pragma once

#include <nn/types.h>

namespace nn::font {

// Tree links and cache flags are read by GlyphTreeMap. ScalableFont's glyph and width queries
// establish the request mask and three width fields; the list and remaining payload are unknown.
struct GlyphNode {
    static u8 CalculateLineKind(u16 height);

    // The live-list previous link and next free node share this object-pointer slot.
    // The live-list interface and remaining prefix are not established.
    void* _0;
    u8 _8[0x18];
    GlyphNode* left;
    GlyphNode* right;
    u64 key;
    u8 flags;
    u8 color;
    u8 _3a[2];
    u32 requestMask;
    u16 _40;
    u16 _42;
    u16 _44;
    u16 _46;
    u16 glyphWidth;
    u16 _4a;
    u16 charWidth;
    s16 leftWidth;
    u16 _50;
    u8 _52[6];
};
static_assert(sizeof(GlyphNode) == 0x58);

}  // namespace nn::font
