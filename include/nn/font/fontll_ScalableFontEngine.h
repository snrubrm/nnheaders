#pragma once

#include <nn/types.h>

namespace nn::fontll {

// The native import table names this external engine and its constructor. TextureCache::Initialize
// allocates and constructs the array at a 0xb8-byte stride; its internal fields are not known.
class ScalableFontEngine {
public:
    ScalableFontEngine();
    // The boolean result is inferred from several native callers testing bit 0 for glyph existence.
    bool CheckGlyphExist(u32 code);

private:
    u8 _opaque[0xb8];
};
static_assert(sizeof(ScalableFontEngine) == 0xb8);

}  // namespace nn::fontll
