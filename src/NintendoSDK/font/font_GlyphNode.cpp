#include <nn/font/font_GlyphNode.h>

namespace nn::font {

// 0x7100ab3684
u8 GlyphNode::CalculateLineKind(u16 height) {
    if (height > 1024)
        return 0;
    if (height > 512)
        return 10;
    if (height > 256)
        return 9;
    if (height > 128)
        return 8;
    if (height > 64)
        return 7;
    if (height > 32)
        return 6;
    return height > 16 ? 5 : 4;
}

}  // namespace nn::font
