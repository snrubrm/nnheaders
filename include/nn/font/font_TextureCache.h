/**
 * @file font_TextureCache.h
 * @brief Cache of rendered glyphs of scalable fonts (declarations used by ScalableFont).
 */

#pragma once

#include <nn/font/font_TextureObject.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Texture.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>

namespace nn::fontll {
class ScalableFontEngine;
}

namespace nn::font {

struct GlyphNode;

// The glyph index of the cache (constructor 0x7100ab3704, Initialize 0x7100ab3714: it allocates
// `count` fixed 0x58-byte nodes and chains them; only the root / array / count fields are known).
class GlyphTreeMap {
public:
    GlyphTreeMap();

    void Initialize(void* (*allocate)(size_t, size_t, void*), void* user_data, u32 count);
    GlyphNode* Find(u32 code, u16 size, u16 face) const;
    static void UpdateFlagsForCompleteTextureCacheRecursive(GlyphNode* node);
    static GlyphNode* EraseMin(GlyphNode* node);

private:
    /* 0x00 */ GlyphNode* _0;
    /* 0x08 */ void* mNodes;
    /* 0x10 */ void* _10;
    /* 0x18 */ u32 mCount;
};
static_assert(sizeof(GlyphTreeMap) == 0x20);

class TextureCache {
public:
    // Per-font normalized metrics produced by Initialize and read by ScalableFont.
    struct FontMetrics {
        f32 ascent;
        f32 height;
        f32 baseline;
        f32 _c;
        f32 _10;
        f32 _14;
        f32 _18;
    };
    static_assert(sizeof(FontMetrics) == 0x1c);

    // The allocator the font library uses for every cache allocation
    // (TextureCache::Initialize 0x7100ab0fa4 and GlyphTreeMap::Initialize both call it).
    using AllocateFunction = void* (*)(size_t size, size_t alignment, void* user_data);

    // 0x4dd0 bytes: TextureCache::InitializeArg::SetDefault (0x7100ab0c10) writes every field, and
    // Initialize (0x7100ab0fa4) reads them back; the per-face arrays are indexed by
    // [face][slot] with face < 32 and slot < fonts_per_face[face] (at most 8).
    struct InitializeArg {
        void SetDefault();

        /* 0x0000 */ u32 textureWidth;   // 0x400; Initialize requires a power of two
        /* 0x0004 */ u32 textureHeight;  // 0x400
        /* 0x0008 */ AllocateFunction allocate;  // must be non-null
        /* 0x0010 */ void* allocateUserData;
        /* 0x0018 */ void* fontData[32][8];
        /* 0x0818 */ u64 _818[32][8];  // Initialize reads the low u32 (the key of fontll helper Decode)
        /* 0x1018 */ f32 _1018[32][8];  // Initialize stores it times 65536 into the per-font record
        /* 0x1418 */ u8 _1418[32][8];  // Initialize stores it into the per-font record
        /* 0x1518 */ f32 _1518[32][8];  // 1.0f default; Initialize copies it into the font metrics (+10)
        /* 0x1918 */ f32 _1918[32][8];  // 1.0f default; Initialize copies it into the font metrics (+14)
        /* 0x1d18 */ u32 _1d18[32][8];  // font kind: (& ~1) == 2 uses fontll helper Decode, (& | 2) == 3
                                        // reads the OTF kerning table
        /* 0x2118 */ f32 _2118[32][8];  // Initialize copies it into the font metrics (+18)
        /* 0x2518 */ u32 _2518[32][8];  // Initialize stores it into the per-font record
        /* 0x2918 */ u32 _2918[32][8][4];  // Initialize stores the four words into the per-font record
        /* 0x3918 */ u32 _3918[32][8][4];  // Initialize stores the four words into the per-font record
        /* 0x4918 */ u32 fontFaceCount;  // at most 32
        /* 0x491c */ u32 fontsPerFace[32];  // 1 to 8
        /* 0x499c */ u32 _499c;
        /* 0x49a0 */ u64 _49a0;  // 0x80000 default; the ScalableFontEngine::Initialize size
        /* 0x49a8 */ u64 _49a8;  // 0xc800 default; the ScalableFontEngine::Initialize size (times the
                                 // total font count)
        /* 0x49b0 */ u32 glyphTreeCount;  // 0x400 default; the GlyphTreeMap::Initialize count
        /* 0x49b4 */ u32 _49b4[32][8];  // the last LoadFont argument
        /* 0x4db4 */ u8 _4db4;   // stored at cache + 0x18f0
        /* 0x4db8 */ u32 _4db8;  // 1 default; the engine count of the cache
        /* 0x4dc0 */ u32 (*getCurrentCoreNumberMaybe)();  // null default; Initialize substitutes
                                                           // nn::os::GetCurrentCoreNumber
        /* 0x4dc8 */ u8 autoHint;  // Initialize calls ScalableFontEngine::SetAutoHint when set
        /* 0x4dc9 */ u8 _4dc9;     // stored at cache + 0x1938
    };
    static_assert(sizeof(InitializeArg) == 0x4dd0);

    TextureCache();
    virtual ~TextureCache();

    void Initialize(gfx::Device* device, const InitializeArg& arg, gfx::MemoryPool* pool,
                    s64 pool_offset, u64 pool_size);

    void RegisterTextureViewToDescriptorPool(
        bool (*register_function)(gfx::DescriptorSlot*, const gfx::TextureView&, void*),
        void* user_data);

    void UpdateTextureCache();
    void CompleteTextureCache();
    void ResetTextureCache();
    bool IsBorderEffectEnabled(u16 font_face) const;
    const FontMetrics* GetFontMetrics(u16 font_face) const;
    bool RegisterGlyph(u32 code, u32 font_size, u16 font_face, s32 index);
    bool IsGlyphExistInFont(u32 code, u16 font_face);
    s32 CalculateKerning(u32 first, u32 second, u32 font_size, u16 font_face);
    s32 CalculateCharWidth(u32 code, u32 font_size, u16 font_face);
    GlyphNode* FindGlyphNode(u32 code, u32 font_size, u16 font_face);

private:
    friend class ScalableFont;
    void SetFontFaceNoPlot(u32 font, u32 core);

    // The four intrusive list heads of a face node and the head of a list; the constructor
    // (0x7100ab0d24) self-links all of them. Names are guesses.
    struct ListHead {
        // Inline-only; name is a guess. TextureCache's constructor self-links every head.
        ListHead() : prev(this), next(this) {}

        /* 0x00 */ void* prev;
        /* 0x08 */ void* next;
    };

    // One glyph management node of the cache (0x60 bytes, 64 of them at +0x80). Name is a guess.
    struct FaceNode {
        /* 0x00 */ ListHead head0;
        /* 0x10 */ u64 _10;
        /* 0x18 */ ListHead head18;
        /* 0x28 */ u64 _28;
        /* 0x30 */ ListHead head30;
        /* 0x40 */ u64 _40;
        /* 0x48 */ ListHead head48;
        /* 0x58 */ u64 _58;
    };
    static_assert(sizeof(FaceNode) == 0x60);

    /* 0x08 */ fontll::ScalableFontEngine* mScalableFontEngine;
    /* 0x10 */ fontll::ScalableFontEngine* mScalableFontEngines;
    /* 0x18 */ u64 _18;
    /* 0x20 */ void* mTextureData;
    /* 0x28 */ u32 mTextureWidth;
    /* 0x2c */ u32 mTextureHeight;
    /* 0x30 */ GlyphTreeMap mGlyphTreeMap;
    /* 0x50 */ ListHead _50[3];
    /* 0x80 */ FaceNode mFaceNodes[64];
    /* 0x1880 */ u32 _1880;
    /* 0x1884 */ u32 mFontFaceCount;  // Initialize stores arg.fontFaceCount here
    /* 0x1888 */ u8 _1888[32];
    /* 0x18a8 */ u8 _18a8[32];
    /* 0x18c8 */ u32 mFontFace;  // SetFontFace caches the face here
    /* 0x18cc */ u32 _18cc;
    /* 0x18d0 */ u64 _18d0;
    /* 0x18d8 */ u64 _18d8;
    /* 0x18e0 */ u64 _18e0;
    /* 0x18e8 */ u32 _18e8;
    /* 0x18ec */ u32 _18ec;
    /* 0x18f0 */ u8 _18f0;
    /* 0x18f4 */ u32 _18f4;
    /* 0x18f8 */ u32 (*_18f8)();
    /* 0x1900 */ FontMetrics* mFontMetrics;
    /* 0x1908 */ void* _1908;
    /* 0x1910 */ u8* mBorderFlags;
    /* 0x1918 */ s32* mRangeCounts;
    /* 0x1920 */ u32 (*mRangeStarts)[4];
    /* 0x1928 */ u32 (*mRangeEnds)[4];
    /* 0x1930 */ void* _1930;
    /* 0x1938 */ u8 _1938;
    /* 0x1940 */ gfx::detail::TextureImpl<gfx::ApiVariationNvn8> mTexture;
    /* 0x1a18 */ gfx::TextureView mTextureView;
    /* 0x1a60 */ gfx::detail::MemoryPoolImpl<gfx::ApiVariationNvn8> mMemoryPool;
    /* 0x1b80 */ gfx::MemoryPool* mPool;
    /* 0x1b88 */ s64 mPoolOffset;
    /* 0x1b90 */ PlacementTextureObject mTextureObject;
};
static_assert(sizeof(TextureCache) == 0x1bb8);

}  // namespace nn::font
