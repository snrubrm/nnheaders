#include <nn/font/font_Font.h>

namespace nn::font {

// 0x7100ab3568
Font::Font() : mKerningEnabled(true), _9(true) {}

// 0x7100ab3584
Font::~Font() {}

// 0x7100ab3680
void Font::Finalize(gfx::Device*) {}

}  // namespace nn::font
