#pragma once

#include <nn/gfx/gfx_ResTexture.h>
#include <nn/ui2d/TextureInfo.h>

namespace nn::ui2d {

// A texture reference into a texture archive (nn::gfx::ResTexture), filled by ResourceAccessor::LoadTexture
// (ui2d::LoadTexture 0x7100abc670 stores the resource at +0x10). Embedded in eui::MultiArcResourceAccessor's
// TextureLink (vtable 0x24c7f78, virtuals 0x7100be0e4c..0x7100be0fa8). The resource member is not set by the
// constructor in the original (TextureLink's constructor leaves it uninitialised).
class ResourceTextureInfo : public TextureInfo {
public:
    NN_RUNTIME_TYPEINFO(TextureInfo)

    ResourceTextureInfo() = default;
    ~ResourceTextureInfo() override = default;

    void Finalize(gfx::Device* device) override;
    TexSize GetSize() const override;
    u32 m5() const override;
    bool IsValid() const override;
    const gfx::TextureView* GetTextureView() const override;
    gfx::TextureView* GetTextureView() override;

private:
    friend bool LoadTexture(ResourceTextureInfo*, gfx::Device*, const void*);

    gfx::ResTexture* mResource;
};
static_assert(sizeof(ResourceTextureInfo) == 0x18);

}  // namespace nn::ui2d
