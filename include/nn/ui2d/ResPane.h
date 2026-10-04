#pragma once

#include <nn/ui2d/ResExtUserData.h>
#include <nn/ui2d/Types.h>
#include <nn/util/MathTypes.h>
#include <cstddef>

namespace nn::ui2d {

// Fixed pane resource header, read by Pane's resource constructor (0xab7f7c).
// Resource field names are descriptive guesses.
class ResPane {
public:
    ResBlockHeader mBlockHeader;
    u8 mFlags;
    u8 mBasePosition;
    u8 mAlpha;
    u8 mFlagsEx;
    char mName[24];
    char mUserData[8];
    util::Float3 mTranslation;
    util::Float3 mRotation;
    util::Float2 mScale;
    Size mSize;
};
static_assert(sizeof(ResPane) == 0x54);
static_assert(offsetof(ResPane, mTranslation) == 0x2c);
static_assert(offsetof(ResPane, mSize) == 0x4c);

// Known override-data prefix used by Pane's resource constructor. Its full
// resource extent is not established; no allocation uses sizeof this type.
struct ResPaneBasicInfo {
    char mUserData[8];
    util::Float3 mTranslation;
    util::Float3 mRotation;
    util::Float2 mScale;
    Size mSize;
    u8 mAlpha;
};
static_assert(offsetof(ResPaneBasicInfo, mAlpha) == 0x30);

// Fixed 'prt1' prefix. BuildPartsImpl (0xab68c0) consumes the count and scale;
// 0x28-byte replacement entries and the layout-name string follow this header.
struct ResParts : ResPane {
    u32 mPartsCount;
    util::Float2 mPartsScale;
};
static_assert(sizeof(ResParts) == 0x60);

// Fixed text-box resource header, read by TextBox's constructor (0xaba5b8).
// The string, its ID and optional arrays are addressed by relative offsets.
struct ResTextBox : ResPane {
    u16 mTextBufferBytes;
    u16 mTextBytes;
    u16 mMaterialIndex;
    u16 mFontIndex;
    u8 mTextPosition;
    u8 mLineAlignment;
    u8 mTextFlags;
    f32 mItalicRatio;
    u32 mTextOffset;
    util::Unorm8x4 mTextColors[2];
    Size mFontSize;
    f32 mCharSpace;
    f32 mLineSpace;
    u32 mTextIdOffset;
    util::Float2 mShadowOffset;
    util::Float2 mShadowScale;
    util::Unorm8x4 mShadowColors[2];
    f32 mShadowItalicRatio;
    u32 _a0Offset;
    u32 mPerCharacterTransformOffset;
};
static_assert(sizeof(ResTextBox) == 0xa8);
static_assert(offsetof(ResTextBox, mTextBufferBytes) == 0x54);
static_assert(offsetof(ResTextBox, mTextOffset) == 0x64);
static_assert(offsetof(ResTextBox, mTextIdOffset) == 0x80);
static_assert(offsetof(ResTextBox, mPerCharacterTransformOffset) == 0xa4);

}  // namespace nn::ui2d
