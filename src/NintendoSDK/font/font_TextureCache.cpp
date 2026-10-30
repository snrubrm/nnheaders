#include <nn/font/font_TextureCache.h>
#include <nn/font/font_GlyphNode.h>
#include <nn/font/fontll_ScalableFontEngine.h>

namespace nn::font {

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

// 0x7100ab0d24
TextureCache::TextureCache()
    : mScalableFontEngine(nullptr), mScalableFontEngines(nullptr), mFontData(nullptr),
      mTextureData(nullptr), mTextureWidth(0), mTextureHeight(0), mLineCount(0), mFontFaceCount(0),
      mFontFace(0), _18ec(0),
      _18f0(0), _18f4(1), _18f8(nullptr), mFontMetrics(nullptr), _1938(1), mPool(nullptr),
      mPoolOffset(0) {
    for (s32 i = 0; i < 32; ++i) {
        _1888[i] = 0;
        _18a8[i] = 0;
    }
}

// 0x7100ab0e7c
TextureCache::~TextureCache() = default;

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

// NON_MATCHING: native tests for an empty range list before its signed count; the search's
// branches and selected-font register allocation differ.
GlyphNode* TextureCache::FindGlyphNode(u32 code, u32 font_size, u16 font_face) {
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
                return mGlyphTreeMap.Find(code, font_size, font);
        }
    }
    return nullptr;
}

// NON_MATCHING: font-index allocation and range-base arithmetic scheduling differ.
// 0x7100ab2afc
s32 TextureCache::CalculateCharWidth(u32 code, u32 font_size, u16 font_face) {
    u32 font = _18a8[font_face];
    for (s32 i = 0; i < _1888[font_face]; ++i, ++font) {
        const s32 range_count = mRangeCounts[font];
        if (range_count != 0) {
            s32 range = 0;
            for (; range < range_count; ++range) {
                if (mRangeStarts[font][range] <= code && code <= mRangeEnds[font][range])
                    break;
            }
            if (range >= range_count)
                continue;
        }
        const u32 check_core = _18f0 ? _18f8() : 0;
        SetFontFaceNoPlot(font, check_core);
        if (!mScalableFontEngines[check_core].CheckGlyphExist(code))
            continue;

        const u32 core = _18f0 ? _18f8() : 0;
        SetFontFaceNoPlot(font, core);
        const s32 scale = s32(font_size << 16);
        mScalableFontEngines[core].SetScale(scale, 0, 0, scale);
        s16 advance;
        s16 out1;
        s32 out2;
        s32 out3;
        if (mScalableFontEngines[core].GetAdvance(&advance, &out1, &out2, &out3, code, 8) == 0)
            return advance;
        return 0;
    }
    return 0;
}

// NON_MATCHING: font-search exit bookkeeping and final status selection differ.
// 0x7100ab2d58
s32 TextureCache::CalculateKerning(u32 first, u32 second, u32 font_size, u16 font_face) {
    if (first == 0) {
        u32 font = _18a8[font_face];
        for (s32 i = 0; i < _1888[font_face]; ++i, ++font) {
            const s32 range_count = mRangeCounts[font];
            if (range_count != 0) {
                s32 range = 0;
                for (; range < range_count; ++range) {
                    if (mRangeStarts[font][range] <= second && second <= mRangeEnds[font][range])
                        break;
                }
                if (range >= range_count)
                    continue;
            }
            const u32 check_core = _18f0 ? _18f8() : 0;
            SetFontFaceNoPlot(font, check_core);
            if (!mScalableFontEngines[check_core].CheckGlyphExist(second))
                continue;

            const u32 core = _18f0 ? _18f8() : 0;
            SetFontFaceNoPlot(font, core);
            const s32 scale = s32(font_size << 16);
            mScalableFontEngines[core].SetScale(scale, 0, 0, scale);
            const fontll::OtfKerningTable* table = _1930[font];
            if (table)
                return mScalableFontEngines[core].AcquireOtfKerningFirst(table, second, font_size);
            return 0;
        }
        return 0;
    }
    if (second == 0) {
        u32 font = _18a8[font_face];
        for (s32 i = 0; i < _1888[font_face]; ++i, ++font) {
            const s32 range_count = mRangeCounts[font];
            if (range_count != 0) {
                s32 range = 0;
                for (; range < range_count; ++range) {
                    if (mRangeStarts[font][range] <= first && first <= mRangeEnds[font][range])
                        break;
                }
                if (range >= range_count)
                    continue;
            }
            const u32 check_core = _18f0 ? _18f8() : 0;
            SetFontFaceNoPlot(font, check_core);
            if (!mScalableFontEngines[check_core].CheckGlyphExist(first))
                continue;

            const u32 core = _18f0 ? _18f8() : 0;
            SetFontFaceNoPlot(font, core);
            const s32 scale = s32(font_size << 16);
            mScalableFontEngines[core].SetScale(scale, 0, 0, scale);
            const fontll::OtfKerningTable* table = _1930[font];
            if (table)
                return mScalableFontEngines[core].AcquireOtfKerningLast(table, first, font_size);
            return 0;
        }
        return 0;
    }

    u32 first_font = _18a8[font_face];
    bool first_found = false;
    for (s32 i = 0; i < _1888[font_face]; ++i, ++first_font) {
        const s32 range_count = mRangeCounts[first_font];
        if (range_count != 0) {
            s32 range = 0;
            for (; range < range_count; ++range) {
                if (mRangeStarts[first_font][range] <= first && first <= mRangeEnds[first_font][range])
                    break;
            }
            if (range >= range_count)
                continue;
        }
        const u32 core = _18f0 ? _18f8() : 0;
        SetFontFaceNoPlot(first_font, core);
        if (mScalableFontEngines[core].CheckGlyphExist(first)) {
            first_found = true;
            break;
        }
    }
    if (!first_found)
        return 0;

    u32 second_font = _18a8[font_face];
    bool second_found = false;
    for (s32 i = 0; i < _1888[font_face]; ++i, ++second_font) {
        const s32 range_count = mRangeCounts[second_font];
        if (range_count != 0) {
            s32 range = 0;
            for (; range < range_count; ++range) {
                if (mRangeStarts[second_font][range] <= second && second <= mRangeEnds[second_font][range])
                    break;
            }
            if (range >= range_count)
                continue;
        }
        const u32 core = _18f0 ? _18f8() : 0;
        SetFontFaceNoPlot(second_font, core);
        if (mScalableFontEngines[core].CheckGlyphExist(second)) {
            second_found = true;
            break;
        }
    }
    if (!second_found || first_font != second_font)
        return 0;

    const u32 core = _18f0 ? _18f8() : 0;
    SetFontFaceNoPlot(first_font, core);
    const s32 scale = s32(font_size << 16);
    mScalableFontEngines[core].SetScale(scale, 0, 0, scale);
    const fontll::OtfKerningTable* table = _1930[first_font];
    if (table) {
        const s32 kerning = mScalableFontEngines[core].AcquireOtfKerning(
            table, first, second, font_size);
        if (kerning != 0)
            return kerning;
    }
    s32 out0;
    s32 out1;
    if (mScalableFontEngines[core].GetKerning(&out0, &out1, first, second) != 0)
        return 0;
    return out0 >> 16;
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

// 0x7100ab3a78
// NON_MATCHING: branch merging and an omitted overwritten intermediate rotation store.
GlyphNode* GlyphTreeMap::Erase(GlyphNode* node, u64 key) {
    const bool left_is_red = node->left && node->left->color == 1;
    if (key < node->key) {
        if (!left_is_red && (!node->left->left || node->left->left->color != 1)) {
            node->color = !node->color;
            node->left->color = !node->left->color;
            node->right->color = !node->right->color;
            GlyphNode* right = node->right;
            if (right->left && right->left->color == 1) {
                GlyphNode* middle = right->left;
                right->left = middle->right;
                middle->right = right;
                middle->color = right->color;
                right->color = 1;
                node->right = middle->left;
                middle->left = node;
                middle->color = node->color;
                node->color = 1;
                node = middle;
                node->color = !node->color;
                node->left->color = !node->left->color;
                node->right->color = !node->right->color;
            }
        }
        node->left = Erase(node->left, key);
    } else {
        if (left_is_red) {
            GlyphNode* left = node->left;
            node->left = left->right;
            left->right = node;
            left->color = node->color;
            node->color = 1;
            node = left;
        }
        if (node->key == key && !node->right) {
            node->_0 = mFreeNodes;
            mFreeNodes = node;
            return nullptr;
        }
        if (node->right->color != 1 &&
            (!node->right->left || node->right->left->color != 1)) {
            node->color = !node->color;
            node->left->color = !node->left->color;
            node->right->color = !node->right->color;
            GlyphNode* left = node->left;
            if (left->left && left->left->color == 1) {
                node->left = left->right;
                left->right = node;
                left->color = node->color;
                node->color = 1;
                node = left;
                node->color = !node->color;
                node->left->color = !node->left->color;
                node->right->color = !node->right->color;
            }
        }
        if (node->key == key) {
            GlyphNode* minimum = node->right;
            while (minimum->left)
                minimum = minimum->left;
            const u64 minimum_key = minimum->key;
            GlyphNode* replacement = node->right;
            while (replacement) {
                if (replacement->key > minimum_key)
                    replacement = replacement->left;
                else if (replacement->key < minimum_key)
                    replacement = replacement->right;
                else
                    break;
            }
            replacement->right = EraseMin(node->right);
            replacement->left = node->left;
            replacement->color = node->color;
            node->_0 = mFreeNodes;
            mFreeNodes = node;
            node = replacement;
        } else {
            node->right = Erase(node->right, key);
        }
    }
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

// 0x7100ab3a30
// NON_MATCHING: root load and key packing are scheduled differently.
void GlyphTreeMap::Erase(u32 code, u16 size, u16 face) {
    const u64 key = (static_cast<u64>(size) << 48) | (static_cast<u64>(face) << 32) | code;
    _0 = Erase(_0, key);
    if (_0)
        _0->color = 0;
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

// 0x7100ab1a00
// NON_MATCHING: opaque record indexing uses a widened multiply; scheduling also differs.
void TextureCache::SetFontFace(u32 font) {
    if (mFontFace == font)
        return;
    mScalableFontEngine->SetFont(mFontData[font]);
    mScalableFontEngine->SetBoldWeight(mBoldWeights[font]);
    const u8 outline_width = mBorderFlags[font];
    if (outline_width) {
        mScalableFontEngine->SetOutlineWidth(outline_width);
        mScalableFontEngine->SetFlags(static_cast<fontll::ScalableFontEngine::Flags>(2));
    } else {
        mScalableFontEngine->SetFlags(static_cast<fontll::ScalableFontEngine::Flags>(0));
    }
    mFontFace = font;
}

// 0x7100ab2ca4
// NON_MATCHING: opaque record indexing uses a widened multiply; scheduling also differs.
void TextureCache::SetFontFaceNoPlot(u32 font, u32 core) {
    if (mFontFacesPerCore[core] == font)
        return;
    mScalableFontEngines[core].SetFont(mFontData[font]);
    mScalableFontEngines[core].SetBoldWeight(mBoldWeights[font]);
    const u8 outline_width = mBorderFlags[font];
    if (outline_width) {
        mScalableFontEngines[core].SetOutlineWidth(outline_width);
        mScalableFontEngines[core].SetFlags(static_cast<fontll::ScalableFontEngine::Flags>(2));
    } else {
        mScalableFontEngines[core].SetFlags(static_cast<fontll::ScalableFontEngine::Flags>(0));
    }
    mFontFacesPerCore[core] = font;
}

}  // namespace nn::font
