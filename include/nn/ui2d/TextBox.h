/**
 * @file TextBox.h
 * @brief UI pane that draws text.
 */

#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>
#include <nn/ui2d/Pane.h>
#include <nn/ui2d/TextSearcher.h>
#include <nn/util/MathTypes.h>

namespace nn::font {
class Font;
class DispStringBuffer;
template <typename CharType>
class TagProcessorBase;
template <typename CharType>
class TextWriterBase;
}  // namespace nn::font

namespace nn::ui2d {
class BuildResultInformation;
struct ResTextBox;

// Placeholder name: aligns a byte size for the requested GPU buffer access.
size_t sub_7100ABCB10(gfx::Device* device, int gpu_access, size_t size);

// Layout evidence: TextBox::TextBox (0x7100aba5b8) copies the members out of the ResTextBox, FreeStringBuffer /
// SetString / SetFontSize / SetupTextWriter (0x7100abc000 / abc06c / abc34c / abc3d0) use the string, font and size
// members, and the member set and order follow the NintendoWare layout library's TextBox. Names without a
// stated source are guesses; sizeof is 0x160 (the last member is stored at +0x158).
class TextBox : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane)

    // Filled by the resource constructor (0xaba5b8), consumed by InitializeString.
    struct InitializeStringParam {
        u32 mFlags;
        const Layout* mRootLayout;
        const void* mText;
        size_t mBufferLength;
        s32 mTextLength;
    };
    static_assert(sizeof(InitializeStringParam) == 0x28);

    // The two 16-element float arrays are passed to TextWriterBase::Print by Calculate.
    // Their role is not established; member and type names remain placeholders.
    struct Unk140 {
        f32* _0;
        f32* _8;
    };

    // Record fields follow UpdatePerCharacterTransform's rotation, translation and color targets.
    // Names are descriptive guesses; AllocateStringBuffer constructs one per character.
    struct PerCharacterTransform {
        util::Float3 mRotationCos{{1.0f, 1.0f, 1.0f}};
        util::Float3 mRotationSin{{0.0f, 0.0f, 0.0f}};
        util::Float3 mTranslation{{0.0f, 0.0f, 0.0f}};
        util::Unorm8x4 mColors[2]{{{255, 255, 255, 255}}, {{255, 255, 255, 255}}};
    };
    static_assert(sizeof(PerCharacterTransform) == 0x2c);

    // Per-character animation state, populated from the resource by the constructor and
    // read by UpdatePerCharacterTransform. Resource entries may extend past the first one.
    struct Unk158 {
        f32 _0;
        f32 _4;
        PerCharacterTransform* _8;
        u8 _10;
        u8 _11;
        u8 _12;
        struct Entry {
            const void* _0;
            s32 _8;
        };
        Entry _18[1];
    };
    static_assert(sizeof(Unk140) == 0x10);
    static_assert(sizeof(Unk158) == 0x28);

    TextBox(BuildResultInformation*, gfx::Device*, InitializeStringParam*, const ResTextBox*,
            const ResTextBox*, const BuildArgSet&);
    TextBox(const TextBox&, gfx::Device*);
    ~TextBox() override;

    void Finalize(gfx::Device*) override;
    util::Unorm8x4 GetVertexColor(s32) const override;
    void SetVertexColor(s32, const util::Unorm8x4&) override;
    u8 GetVertexColorElement(s32) const override;
    void SetVertexColorElement(s32, u8) override;
    u8 GetMaterialCount() const override;
    Material* GetMaterial(s32) const override;
    void Calculate(DrawInfo&, CalculateContext&, bool) override;
    void DrawSelf(DrawInfo&, gfx::CommandBuffer&) override;

    virtual void InitializeString(BuildResultInformation*, gfx::Device*, const BuildArgSet&,
                                  const InitializeStringParam&);
    // The two buffer sizes: the buffer length (characters) and, in the second form, the string length to reserve
    // (names after nw::lyt::TextBox).
    virtual void AllocateStringBuffer(gfx::Device*, u16 length);
    virtual void AllocateStringBuffer(gfx::Device*, u16 length, u16 string_length);
    virtual void FreeStringBuffer(gfx::Device*);
    // Copies a string into the buffer at `dst_index`; returns the number of characters copied.
    virtual u16 SetString(const u16* string, u16 dst_index);
    virtual u16 SetStringUtf8(const char* string, u16 dst_index);
    virtual u16 SetString(const u16* string, u16 dst_index, u16 length);
    virtual u16 SetStringUtf8(const char* string, u16 dst_index, u16 length);
    virtual void SetupTextWriter(font::TextWriterBase<u16>*) const;
    virtual void SetupTextWriterUtf8(font::TextWriterBase<char>*) const;
    virtual bool InitializeStringWithTextSearcherInfo(gfx::Device*, const BuildArgSet&,
                                                      const TextSearcher::TextInfo&);
    virtual bool InitializeStringWithTextSearcherInfoUtf8(gfx::Device*, const BuildArgSet&,
                                                          const TextSearcher::TextInfoUtf8&);

    u16 GetStringBufferLength() const;
    const font::Font* GetFont() const;
    const Size& GetFontSize() const { return mFontSize; }
    void SetFontSize(const Size& size);
    void SetTagProcessor(font::TagProcessorBase<u16>* tag_processor) {
        const bool changed = mTagProcessor != tag_processor;
        mBits.textChanged |= changed;
        if (changed)
            mTagProcessor = tag_processor;
    }
    // The text rectangle (0x7100abb2f0).
    void GetTextDrawRect() const;

    // Horizontal text position (HorizontalPosition_*; the low two bits of mTextPosition).
    HorizontalPosition GetTextPositionH() const {
        return static_cast<HorizontalPosition>(mTextPosition & 3);
    }

protected:
    void LoadMtx(DrawInfo&) override;

public:
    // The bits at +0x11c (TextBox::TextBox fills them from the resource's flags). `textChanged` is set whenever the
    // text, font size or a color changes (the text needs to be laid out again).
    struct Bits {
        u8 textAlignment : 2;
        u8 textChanged : 1;
        u8 _3 : 1;
        u8 _4 : 1;
        u8 _5 : 1;
        u8 _6 : 1;
        u8 _7 : 1;
    };

    /* 0xe0 */ u16* mTextBuf;
    /* 0xe8 */ const char* mTextId;  // eui::TextBoxEx::isTextChangeOn_ tests for a leading '@'
    /* 0xf0 */ util::Unorm8x4 mTextColors[2];  // top, bottom (GetVertexColor(i) reads [i / 2])
    /* 0xf8 */ const font::Font* mFont;
    /* 0x100 */ Size mFontSize;
    /* 0x108 */ f32 mLineSpace;
    /* 0x10c */ f32 mCharSpace;
    /* 0x110 */ font::TagProcessorBase<u16>* mTagProcessor;  // from the layout (Layout +0x10) unless set
    /* 0x118 */ u16 mTextBufBytes;  // buffer length including the terminator (0: no buffer)
    /* 0x11a */ u16 mTextLength;
    /* 0x11c */ Bits mBits;
    /* 0x11d */ u8 _11d;
    /* 0x11e */ u8 mTextPosition;
    /* 0x11f */ bool mIsUtf8;
    /* 0x120 */ f32 mItalicRatio;  // scaled together with mShadowItalicRatio by SetFontSize
    /* 0x124 */ util::Float2 mShadowOffset;
    /* 0x12c */ util::Float2 mShadowScale;
    /* 0x134 */ u32 mShadowTopColor;
    /* 0x138 */ u32 mShadowBottomColor;
    /* 0x13c */ f32 mShadowItalicRatio;
    /* 0x140 */ Unk140* _140;
    /* 0x148 */ Material* mMaterial;
    /* 0x150 */ font::DispStringBuffer* mDispStringBuf;
    /* 0x158 */ Unk158* _158;
};
static_assert(sizeof(TextBox) == 0x160);

}  // namespace nn::ui2d
