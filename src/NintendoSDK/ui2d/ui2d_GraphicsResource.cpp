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

}  // namespace nn::ui2d
