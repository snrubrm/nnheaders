#pragma once

#include <nn/g3d/ModelAnimObj.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::g3d {

// Known inheritance prefix for pointer use only. Native type information at
// 0x710252cab0 names MaterialAnimObj and its direct ModelAnimObj base at
// 0x710252ca20. The material-type-3 producer 0x7100bfe96c installs that type.
// Additional state and virtual contracts are not modelled; this declaration
// does not establish the total extent or support allocation/copy/lifetime code.
class MaterialAnimObj : public ModelAnimObj {
public:
    MaterialAnimObj();
    ~MaterialAnimObj() override;

    // 0x710132d1ec: the release build does nothing.
    void ClearResult() override;

    // AGL MaterialAnimObj::SetResTexture 0x7100b33ff4 loads both array bases
    // and stores the ResTexture view and descriptor slot through them, exactly
    // like the runtime ResMaterial arrays at +0x30/+0x98.
    u8 _68[0x90 - 0x68];
    const gfx::TextureView** mTextureViews;  // 90
    gfx::DescriptorSlot* mTextureDescriptorSlots;  // 98
};

}  // namespace nn::g3d
