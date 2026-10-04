#include <nn/font/font_ResFont.h>

namespace nn::font {

// 0x7101325fdc
ResFont::ResFont() {}

// The original keeps the vtable store (a plain empty body drops it); `{ ; }` as in upstream's
// GameDataFlagSelector::~GameDataFlagSelector (commit 96101229).
// 0x710132600c
ResFont::~ResFont() {
    ;
}

}  // namespace nn::font
