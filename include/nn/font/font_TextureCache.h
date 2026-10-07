/**
 * @file font_TextureCache.h
 * @brief Cache of rendered glyphs of scalable fonts (declarations used by ScalableFont).
 */

#pragma once

#include <nn/types.h>

namespace nn::font {

struct GlyphNode;

class TextureCache {
public:
    struct InitializeArg;

    void UpdateTextureCache();
    void CompleteTextureCache();
    void ResetTextureCache();
    bool IsBorderEffectEnabled(u16 font_face) const;
    bool IsGlyphExistInFont(u32 code, u16 font_face);
    s32 CalculateKerning(u32 first, u32 second, u32 font_size, u16 font_face);
    s32 CalculateCharWidth(u32 code, u32 font_size, u16 font_face);
    GlyphNode* FindGlyphNode(u32 code, u32 font_size, u16 font_face);
};

}  // namespace nn::font
