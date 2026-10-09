#pragma once

#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::g3d {

// Native texture callbacks return the view and descriptor in x0/x1 (b33dbc).
// ResMaterial and ResMaterialAnim store both words separately (ac895c/ac6020).
struct TextureRef {
    const gfx::TextureView* pTextureView;
    gfx::DescriptorSlot descriptorSlot;
};
static_assert(sizeof(TextureRef) == 0x10);

}  // namespace nn::g3d
