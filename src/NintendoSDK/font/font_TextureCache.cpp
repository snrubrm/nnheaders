#include <nn/font/font_TextureCache.h>

namespace nn::font {

// 0x7100ab1a84
void TextureCache::RegisterTextureViewToDescriptorPool(
    bool (*register_function)(gfx::DescriptorSlot*, const gfx::TextureView&, void*), void* user_data) {
    register_function(mTextureObject.GetDescriptorSlot(), mTextureView, user_data);
}

// 0x7100ab0c10
void TextureCache::InitializeArg::SetDefault() {
    textureWidth = 0x400;
    textureHeight = 0x400;
    allocate = nullptr;
    allocateUserData = nullptr;
    for (u32 face = 0; face < 32; ++face) {
        for (u32 slot = 0; slot < 8; ++slot) {
            fontData[face][slot] = nullptr;
            _818[face][slot] = 0;
            _1018[face][slot] = 0.0f;
            _1418[face][slot] = 0;
            _1518[face][slot] = 1.0f;
            _1918[face][slot] = 1.0f;
            _1d18[face][slot] = 0;
            _2118[face][slot] = 0.0f;
            _2518[face][slot] = 0;
            for (u32 j = 0; j < 4; ++j) {
                _2918[face][slot][j] = _3918[face][slot][j] = 0;
            }
            _49b4[face][slot] = 0;
        }
        fontsPerFace[face] = 1;
    }
    fontFaceCount = 1;
    _49a0 = 0x80000;
    _49a8 = 0xc800;
    glyphTreeCount = 0x400;
    _4db4 = 0;
    _4db8 = 1;
    getCurrentCoreNumberMaybe = nullptr;
    autoHint = false;
    _4dc9 = true;
}

// 0x7100ab0d24
// The constructor body is left undecompiled: interleaving the GlyphTreeMap / gfx impl /
// TextureObject constructions with the scalar stores needs more evidence to model honestly.

}  // namespace nn::font
