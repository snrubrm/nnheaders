#include <nn/font/font_RectDrawer.h>

#include <nn/font/font_GpuBuffer.h>
#include <nn/gfx/gfx_GpuAddress.h>

#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/detail/gfx_ResShaderImpl.h>
#include <nn/gfx/gfx_Shader.h>
#include <nn/gfx/gfx_Device.h>

namespace nn::font {

RectDrawer::RectDrawer() : mMaxCharCount(0) {}

RectDrawer::~RectDrawer() = default;

void RectDrawer::Finalize(gfx::Device* device) {
    if (mMaxCharCount < 1)
        return;

    if (mInterpolationDisabledBuffer.ToData()->state)
        mInterpolationDisabledBuffer.Finalize(device);
    if (mInterpolationEnabledBuffer.ToData()->state)
        mInterpolationEnabledBuffer.Finalize(device);
    if (mVertexBuffer.ToData()->state)
        mVertexBuffer.Finalize(device);
    if (mIndexBuffer.ToData()->state)
        mIndexBuffer.Finalize(device);
    if (mInterpolationDisabledMemoryPool.ToData()->state)
        mInterpolationDisabledMemoryPool.Finalize(device);
    if (mInterpolationEnabledMemoryPool.ToData()->state)
        mInterpolationEnabledMemoryPool.Finalize(device);
    if (mVertexMemoryPool.ToData()->state)
        mVertexMemoryPool.Finalize(device);
    if (mSampler.ToData()->state)
        mSampler.Finalize(device);
    for (auto& vertex_state : mVertexStates) {
        if (vertex_state.ToData()->state)
            vertex_state.Finalize(device);
    }

    auto* container = mResShaderFile->GetShaderContainer();
    for (int i = 0; i < 6; ++i) {
        gfx::Shader* shader = container->GetResShaderVariation(i)
                                  ->GetResShaderProgram(static_cast<gfx::ShaderCodeType>(mShaderSelection))
                                  ->GetShader();
        shader->Finalize(device);
    }
    gfx::detail::ResShaderContainerImpl::Finalize<gfx::DefaultApi>(container, device);
    mMaxCharCount = 0;
}

// 0x7101325344
void RectDrawer::Draw(gfx::CommandBuffer& command_buffer, const DispStringBuffer& buffer) const {
    if (buffer.mCapacity < 1 || (buffer._b0->mFlags & 2) || mMaxCharCount < buffer.mCharCount)
        return;

    gfx::GpuAddress constant_address{};
    gfx::GpuAddress vertex_address{};
    const auto& vertex_data = buffer.mVertexBufferData;
    buffer._b0->mBuffers[buffer._b0->mBufferIndex].GetGpuAddress(&constant_address);
    constant_address.Offset(buffer.mConstantBufferOffset);
    buffer._b0->mBuffers[buffer._b0->mBufferIndex].GetGpuAddress(&vertex_address);
    vertex_address.Offset(buffer.mVertexBufferOffset);

    if (buffer._b9) {
        const u32 flags = buffer.mFlags;
        AddDrawCommand(command_buffer, vertex_data, flags & ~1u, constant_address, vertex_address);
        const size_t alignment = buffer._b0->mBufferAlignment;
        constant_address.Offset((128 + alignment - 1) & -alignment);
        AddDrawCommand(command_buffer, vertex_data, flags | 1u, constant_address, vertex_address);
    } else {
        AddDrawCommand(command_buffer, vertex_data, buffer.mFlags, constant_address, vertex_address);
    }
}

void RectDrawer::RegisterSamplerSlot(SamplerRegistrationCallback callback, void* user_data) {
    callback(&mSamplerSlot, mSampler, user_data);
}

void RectDrawer::UnregisterSamplerSlot(SamplerDescriptorCallback callback, void* user_data) {
    callback(&mSamplerSlot, mSampler, user_data);
    mSamplerSlot.Invalidate();
}

}  // namespace nn::font
