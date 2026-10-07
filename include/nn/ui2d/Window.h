#pragma once

#include <nn/ui2d/Pane.h>
#include <nn/ui2d/detail/TexCoordArray.h>

namespace nn::ui2d {
struct BuildResultInformation;
struct ResWindow;

// Resource/copy producers (abcb70/abd120) and Finalize (abd3cc) establish
// the layout. Private descriptive member names remain reconstruction guesses.
class Window : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane)

    Window(BuildResultInformation*, gfx::Device*, const ResWindow*, const ResWindow*,
           const BuildArgSet&);
    Window(const Window&, gfx::Device*);
    ~Window() override;

    void Finalize(gfx::Device*) override;
    util::Unorm8x4 GetVertexColor(s32) const override;
    void SetVertexColor(s32, const util::Unorm8x4&) override;
    u8 GetVertexColorElement(s32) const override;
    void SetVertexColorElement(s32, u8) override;
    u8 GetMaterialCount() const override;
    Material* GetMaterial(s32) const override;
    Material* FindMaterialByName(const char*, bool) override;
    const Material* FindMaterialByName(const char*, bool) const override;
    void Calculate(DrawInfo&, CalculateContext&, bool) override;
    void DrawSelf(DrawInfo&, gfx::CommandBuffer&) override;

protected:
    struct Frame {
        u8 mTextureFlip;
        Material* mMaterial;
    };

    // abd038 copies the two four-u16 resource groups to +0xda and +0xe2;
    // the first group therefore legitimately reuses Pane's base tail padding.
    u16 mFrameMargins[4];
    u16 mContentMargins[4];
    // Unidentified original interval, not an asserted reserved/alignment field.
    // Both producers place the following colors at +0xf0.
    u8 _ea[6];
    util::Unorm8x4 mVertexColors[4];
    detail::TexCoordArray mTexCoordArray;
    u8 mWindowKind;
    s8 mFrameCount;
    u8 mWindowFlags;
    // Constructors allocate 0x10-byte Frames; Finalize visits Material at +8.
    Frame* mFrames;
    Material* mContentMaterial;
    // DrawSelf abe02c reads this u32 table to select frame constant-buffer offsets.
    u32* mFrameConstantBufferOffsets;
    u32 mFrameConstantBufferCount;
};

static_assert(sizeof(Window) == 0x138);

}  // namespace nn::ui2d
