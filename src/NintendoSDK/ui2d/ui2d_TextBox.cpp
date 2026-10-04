#include <nn/ui2d/TextBox.h>

#include <nn/font/font_DispStringBuffer.h>
#include <nn/ui2d/Layout.h>
#include <nn/ui2d/Material.h>

#include <cstring>
#include <new>
#include <string>

namespace nn::ui2d {

// 0x7100abae48
// NON_MATCHING: shadow-vector copy registers and source-material load scheduling.
TextBox::TextBox(const TextBox& source, gfx::Device* device) : Pane(source) {
    mTextId = source.mTextId;
    mFont = source.mFont;
    mFontSize = source.mFontSize;
    mLineSpace = source.mLineSpace;
    mCharSpace = source.mCharSpace;
    mTagProcessor = source.mTagProcessor;
    mTextBufBytes = 0;
    mTextLength = 0;
    mBits = source.mBits;
    mTextPosition = source.mTextPosition;
    mIsUtf8 = source.mIsUtf8;
    mItalicRatio = source.mItalicRatio;
    mShadowOffset = source.mShadowOffset;
    mShadowScale = source.mShadowScale;
    mShadowTopColor = source.mShadowTopColor;
    mShadowBottomColor = source.mShadowBottomColor;
    mShadowItalicRatio = source.mShadowItalicRatio;
    mTextBuf = nullptr;
    mDispStringBuf = nullptr;
    _158 = nullptr;
    _140 = nullptr;
    mMaterial = nullptr;
    mTextColors[0] = source.mTextColors[0];
    mTextColors[1] = source.mTextColors[1];
    if (source._140) {
        _140 = static_cast<Unk140*>(Layout::AllocateMemory(sizeof(Unk140), 4));
        _140->_8 = static_cast<f32*>(Layout::AllocateMemory(sizeof(f32) * 16, 4));
        _140->_0 = static_cast<f32*>(Layout::AllocateMemory(sizeof(f32) * 16, 4));
        for (size_t i = 0; i < 16; ++i) {
            _140->_8[i] = source._140->_8[i];
            _140->_0[i] = source._140->_0[i];
        }
    }
    if (source._158) {
        _158 = static_cast<Unk158*>(Layout::AllocateMemory(sizeof(Unk158), 4));
        std::memcpy(_158, source._158, sizeof(Unk158));
        _158->_8 = nullptr;
    }
    if (source.GetStringBufferLength() != 0) {
        AllocateStringBuffer(device, source.GetStringBufferLength(),
                             source.mDispStringBuf ? source.mDispStringBuf->mCapacity : 0);
        if (mIsUtf8)
            SetStringUtf8(reinterpret_cast<const char*>(source.mTextBuf), 0, source.mTextLength);
        else
            SetString(source.mTextBuf, 0, source.mTextLength);
    }
    void* memory = Layout::AllocateMemory(sizeof(Material), 4);
    mMaterial = memory ? new (memory) Material(*source.mMaterial, device) : nullptr;
}

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

// 0x7100abb1f8
void TextBox::SetVertexColor(s32 index, const util::Unorm8x4& color) {
    util::Unorm8x4& current = mTextColors[index / 2];
    mBits.textChanged |= current.v[0] != color.v[0] || current.v[1] != color.v[1] ||
                         current.v[2] != color.v[2] || current.v[3] != color.v[3];
    current = color;
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

// 0x7100abc06c
u16 TextBox::SetString(const u16* string, u16 dst_index) {
    const s32 length = std::char_traits<u16>::length(string);
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

// 0x7100abc130
u16 TextBox::SetStringUtf8(const char* string, u16 dst_index) {
    const s32 length = std::strlen(string);
    s32 copied = 0;
    if (mFont && mTextBuf) {
        const u16 capacity = GetStringBufferLength();
        if (dst_index < capacity) {
            copied = capacity - dst_index < length ? capacity - dst_index : length;
            std::memcpy(reinterpret_cast<char*>(mTextBuf) + dst_index, string, copied);
            mTextLength = reinterpret_cast<char*>(mTextBuf)[0] == 0 ? 0 : dst_index + copied;
            reinterpret_cast<char*>(mTextBuf)[mTextLength] = 0;
            mBits.textChanged = true;
        }
    }
    return copied;
}

// 0x7100abc298
u16 TextBox::SetStringUtf8(const char* string, u16 dst_index, u16 length) {
    s32 copied = 0;
    if (mFont && mTextBuf) {
        const u16 capacity = GetStringBufferLength();
        if (dst_index < capacity) {
            copied = capacity - dst_index < length ? capacity - dst_index : length;
            std::memcpy(reinterpret_cast<char*>(mTextBuf) + dst_index, string, copied);
            mTextLength = reinterpret_cast<char*>(mTextBuf)[0] == 0 ? 0 : dst_index + copied;
            reinterpret_cast<char*>(mTextBuf)[mTextLength] = 0;
            mBits.textChanged = true;
        }
    }
    return copied;
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
