#include <nn/font/font_TextureCache.h>
#include <nn/font/font_GlyphNode.h>
#include <new>

namespace nn::font {

GlyphTreeMap::GlyphTreeMap() : _0(), mFreeNodes(), mNodeBuffer(), mCount(0) {}

// 0x7100ab3714
// NON_MATCHING: final indexing uses a widened multiply and a different saved register.
void GlyphTreeMap::Initialize(void* (*allocate)(size_t, size_t, void*), void* user_data,
                              u32 count) {
    mCount = count;
    GlyphNode* nodes = new (allocate(sizeof(GlyphNode) * count, 8, user_data)) GlyphNode[count];
    mFreeNodes = nodes;
    const s32 last = s32(count) - 1;
    for (s32 i = 0; i < last; ++i)
        nodes[i]._0 = &nodes[i + 1];
    nodes[last]._0 = nullptr;
    mNodeBuffer = nodes;
}

// 0x7100ab407c
// NON_MATCHING: final indexing is widened; native also repeats the allocation-base store.
void GlyphTreeMap::Reset() {
    const s32 last = s32(mCount) - 1;
    GlyphNode* nodes = mNodeBuffer;
    _0 = nullptr;
    mFreeNodes = nodes;
    for (s32 i = 0; i < last; ++i)
        nodes[i]._0 = &nodes[i + 1];
    nodes[last]._0 = nullptr;
}

}  // namespace nn::font
