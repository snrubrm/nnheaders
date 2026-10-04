#pragma once

#include <nn/ui2d/Pane.h>

namespace nn::ui2d {
struct BuildResultInformation;
struct ResWindow;

// Declaration-only interface recovered from the resource/copy constructors and
// the Window vtable. Instance fields and full extent are not modeled here;
// do not use this partial declaration to allocate a Window.
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
};

}  // namespace nn::ui2d
