#pragma once

#include <nn/types.h>

namespace nn::fontll {

// The native import table names this external engine and its constructor. TextureCache::Initialize
// allocates and constructs the array at a 0xb8-byte stride; its internal fields are not known.
class ScalableFontEngine {
public:
    // The imported name identifies this enum; its 32-bit int ABI representation is inferred.
    enum Flags : int;

    ScalableFontEngine();
    void SetFont(char* data);
    void SetBoldWeight(s32 weight);
    void SetOutlineWidth(u16 width);
    void SetFlags(Flags flags);
    // The boolean result is inferred from several native callers testing bit 0 for glyph existence.
    bool CheckGlyphExist(u32 code);
    // The SetScale return is unused in the observed native calls.
    void SetScale(s32, s32, s32, s32);
    // Imported parameters are exact; the 32-bit status representation is inferred
    // from CalculateCharWidth's comparison with zero. Output meanings remain unclaimed.
    s32 GetAdvance(s16*, s16*, s32*, s32*, u32, u16);

private:
    u8 _opaque[0xb8];
};
static_assert(sizeof(ScalableFontEngine) == 0xb8);

}  // namespace nn::fontll
