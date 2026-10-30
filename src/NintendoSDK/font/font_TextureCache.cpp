#include <nn/font/font_TextureCache.h>
#include <nn/font/font_GlyphNode.h>
#include <nn/font/fontll_ScalableFontEngine.h>

namespace nn::font {

GlyphTreeMap::GlyphTreeMap() : _0(), mNodes(), _10(), mCount(0) {}

// NON_MATCHING: the loop uses a widened index and hoists the per-face base.
bool TextureCache::IsBorderEffectEnabled(u16 font_face) const {
    const u32 count = _1888[font_face];
    if (!count)
        return false;
    const u32 first = _18a8[font_face];
    for (u32 i = 0; i < count; ++i) {
        if (mBorderFlags[first + i])
            return true;
    }
    return false;
}

// 0x7100ab2910
// NON_MATCHING: native branches on an empty range list before testing its signed count;
// this range search produces different branch flow and font-index allocation.
bool TextureCache::IsGlyphExistInFont(u32 code, u16 font_face) {
    u32 font = _18a8[font_face];
    for (s32 i = 0; i < _1888[font_face]; ++i, ++font) {
        const s32 range_count = mRangeCounts[font];
        bool check_font = range_count == 0;
        for (s32 range = 0; range < range_count; ++range) {
            if (mRangeStarts[font][range] <= code && code <= mRangeEnds[font][range]) {
                check_font = true;
                break;
            }
        }
        if (check_font) {
            const u32 core = _18f0 ? _18f8() : 0;
            SetFontFaceNoPlot(font, core);
            if (mScalableFontEngines[core].CheckGlyphExist(code))
                return true;
        }
    }
    return false;
}

// NON_MATCHING: the unrolled loop uses a precomputed remaining count for termination.
const TextureCache::FontMetrics* TextureCache::GetFontMetrics(u16 font_face) const {
    const u32 first = _18a8[font_face];
    const u32 count = _1888[font_face];
    const FontMetrics* metrics = mFontMetrics;
    u32 selected = first;
    if (count > 1) {
        f32 largest = metrics[first]._c;
        for (u32 i = 1; i < count; ++i) {
            if (largest < metrics[first + i]._c) {
                selected = first + i;
                largest = metrics[selected]._c;
            }
        }
    }
    return &metrics[selected];
}

// NON_MATCHING: the compiler merges the final child checks and returns.
// 0x7100ab3e70
GlyphNode* GlyphTreeMap::EraseMin(GlyphNode* node) {
    if (!node->left)
        return nullptr;

    if (node->left->color != 1 &&
        (!node->left->left || node->left->left->color != 1)) {
        node->color = !node->color;
        node->left->color = !node->left->color;
        node->right->color = !node->right->color;
        GlyphNode* right = node->right;
        if (right->left && right->left->color == 1) {
            GlyphNode* left = right->left;
            right->left = left->right;
            left->right = right;
            left->color = right->color;
            right->color = 1;
            node->right = left->left;
            left->left = node;
            left->color = node->color;
            node->color = 1;
            node = left;
            node->color = !node->color;
            node->left->color = !node->left->color;
            node->right->color = !node->right->color;
        }
    }
    node->left = EraseMin(node->left);
    if (node->right && node->right->color == 1) {
        GlyphNode* right = node->right;
        node->right = right->left;
        right->left = node;
        right->color = node->color;
        node->color = 1;
        node = right;
    }
    if (node->left && node->left->color == 1 && node->left->left &&
        node->left->left->color == 1) {
        GlyphNode* left = node->left;
        node->left = left->right;
        left->right = node;
        left->color = node->color;
        node->color = 1;
        node = left;
    }
    if (node->left && node->left->color == 1 && node->right && node->right->color == 1) {
        node->color = !node->color;
        node->left->color = !node->left->color;
        node->right->color = !node->right->color;
    }
    return node;
}

GlyphNode* GlyphTreeMap::Find(u32 code, u16 size, u16 face) const {
    const u64 key = (static_cast<u64>(size) << 48) | (static_cast<u64>(face) << 32) | code;
    GlyphNode* node = _0;
    while (node) {
        if (node->key > key)
            node = node->left;
        else if (node->key < key)
            node = node->right;
        else
            return node;
    }
    return nullptr;
}

void GlyphTreeMap::UpdateFlagsForCompleteTextureCacheRecursive(GlyphNode* node) {
    do {
        node->flags = ((node->flags & 5) != 0 ? 0x10 : 0) | (node->flags & 0x60);
        if (node->left)
            UpdateFlagsForCompleteTextureCacheRecursive(node->left);
        node = node->right;
    } while (node);
}

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
