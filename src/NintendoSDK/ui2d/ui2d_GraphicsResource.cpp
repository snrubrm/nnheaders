#include <nn/ui2d/GraphicsResource.h>

#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_GpuAddress.h>

namespace nn::ui2d {

GraphicsResource::GraphicsResource() = default;

GraphicsResource::~GraphicsResource() = default;

const gfx::BlendState* GraphicsResource::sub_7100AC1158(u32 index) const {
    return &mBlendStates[index];
}

void GraphicsResource::sub_7100AC1244(gfx::CommandBuffer* command_buffer) const {
    gfx::GpuAddress address;
    mBuffer0.GetGpuAddress(&address);
    command_buffer->SetVertexBuffer(0, address, 8, 32);
}

// 0x7100ac16c0
// NON_MATCHING: natural nested sampler loops differ from the native unrolling.
void GraphicsResource::RegisterCommonSamplerSlot(SamplerRegistrationCallback callback, void* user_data) {
    mRectDrawer.RegisterSamplerSlot(callback, user_data);
    for (int wrap_s = 0; wrap_s < 3; ++wrap_s) {
        for (int wrap_t = 0; wrap_t < 3; ++wrap_t) {
            for (int mag_filter = 0; mag_filter < 2; ++mag_filter) {
                for (int min_filter = 0; min_filter < 2; ++min_filter) {
                    int index = wrap_s * 12 + wrap_t * 4 + mag_filter * 2 + min_filter;
                    callback(&_9b8[index], _9b0[index], user_data);
                }
            }
        }
    }
}

// 0x7100ac18bc
// NON_MATCHING: natural nested sampler loops differ from the native unrolling.
void GraphicsResource::UnregisterCommonSamplerSlot(SamplerDescriptorCallback callback, void* user_data) {
    mRectDrawer.UnregisterSamplerSlot(callback, user_data);
    for (int wrap_s = 0; wrap_s < 3; ++wrap_s) {
        for (int wrap_t = 0; wrap_t < 3; ++wrap_t) {
            for (int mag_filter = 0; mag_filter < 2; ++mag_filter) {
                for (int min_filter = 0; min_filter < 2; ++min_filter) {
                    int index = wrap_s * 12 + wrap_t * 4 + mag_filter * 2 + min_filter;
                    gfx::DescriptorSlot& slot = _9b8[index];
                    callback(&slot, _9b0[index], user_data);
                    slot.Invalidate();
                }
            }
        }
    }
}

// 0x7100ac1b30
// NON_MATCHING: the descriptor pointer and computed index receive exchanged registers.
gfx::DescriptorSlot* GraphicsResource::sub_7100AC1B30(TexWrap wrap_s, TexWrap wrap_t,
                                                   TexFilter min_filter, TexFilter mag_filter) {
    return &_9b8[wrap_s * 12 + wrap_t * 4 + min_filter + mag_filter * 2];
}

}  // namespace nn::ui2d
