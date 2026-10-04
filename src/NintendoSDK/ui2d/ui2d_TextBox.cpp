#include <nn/ui2d/TextBox.h>

#include <cstring>

namespace nn::ui2d {

// 0x7100abb098
// The original keeps the vtable store (a plain empty body drops it); `{ ; }` as in upstream's
// GameDataFlagSelector::~GameDataFlagSelector (commit 96101229).
TextBox::~TextBox() {
    ;
}

// 0x7100abb080
u16 TextBox::GetStringBufferLength() const {
    if (mTextBufBytes == 0)
        return 0;
    return mTextBufBytes - 1;
}

// 0x7100abb1e0
util::Unorm8x4 TextBox::GetVertexColor(s32 index) const {
    return mTextColors[index / 2];
}

// 0x7100abb274
u8 TextBox::GetVertexColorElement(s32 index) const {
    return mTextColors[index / 8].v[index % 4];
}

// 0x7100abb2a4
void TextBox::SetVertexColorElement(s32 index, u8 value) {
    u8& element = mTextColors[index / 8].v[index % 4];
    mBits.textChanged |= element != value;
    element = value;
}

// 0x7100abb194
u8 TextBox::GetMaterialCount() const {
    return mMaterial != nullptr;
}

// 0x7100abb1a4
Material* TextBox::GetMaterial(s32 index) const {
    GetMaterialCount();  // result unused (in the target)
    if (index == 0)
        return mMaterial;
    return nullptr;
}

// 0x7100abc1ec
u16 TextBox::SetString(const u16* string, u16 dst_index, u16 length) {
    s32 copied = 0;
    if (mFont && mTextBuf) {
        const u16 capacity = GetStringBufferLength();
        if (dst_index < capacity) {
            copied = capacity - dst_index < length ? capacity - dst_index : length;
            std::memcpy(mTextBuf + dst_index, string, copied * sizeof(u16));
            mTextLength = mTextBuf[0] == 0 ? 0 : dst_index + copied;
            mTextBuf[mTextLength] = 0;
            mBits.textChanged = true;
        }
    }
    return copied;
}

// 0x7100abc344
const font::Font* TextBox::GetFont() const {
    return mFont;
}

// 0x7100abc34c
void TextBox::SetFontSize(const Size& size) {
    const bool changed = !(mFontSize.width == size.width && mFontSize.height == size.height);
    mBits.textChanged |= changed;
    if (changed) {
        const f32 ratio = mFontSize.width / mFontSize.height;
        const f32 italic = mItalicRatio * ratio;
        const f32 shadow_italic = ratio * mShadowItalicRatio;
        mFontSize = size;
        mItalicRatio = italic * (mFontSize.height / mFontSize.width);
        mShadowItalicRatio = shadow_italic * (mFontSize.height / mFontSize.width);
    }
}

// 0x7100abc3cc
void TextBox::LoadMtx(DrawInfo&) {}

// 0x7100abbe28
void TextBox::AllocateStringBuffer(gfx::Device* device, u16 length) {
    AllocateStringBuffer(device, length, length);
}

}  // namespace nn::ui2d
