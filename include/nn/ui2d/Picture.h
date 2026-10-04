/**
 * @file Picture.h
 * @brief UI pane that draws a textured rectangle.
 */

#pragma once

#include <nn/gfx/gfx_Device.h>
#include <nn/ui2d/Pane.h>
#include <nn/ui2d/detail/TexCoordArray.h>
#include <nn/util/MathTypes.h>

namespace nn::ui2d {
class TextureInfo;

// Layout evidence: Picture::Picture (0x7100ab9afc) stores the material at +0xe0, the four vertex colors at +0xe8 and
// initialises the texture coordinate array at +0xf8; Picture::GetMaterialCount (0x7100ab9e40) tests +0xe0.
class Picture : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane)

    ~Picture() override;
    void Finalize(gfx::Device*) override;

    u8 GetMaterialCount() const override;
    Material* GetMaterial(s32) const override;

protected:
    /* 0xe0 */ Material* mMaterial;
    /* 0xe8 */ util::Unorm8x4 mVertexColors[4];
    /* 0xf8 */ detail::TexCoordArray mTexCoordArray;
};

}  // namespace nn::ui2d
