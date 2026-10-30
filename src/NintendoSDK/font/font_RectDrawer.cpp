#include <nn/font/font_RectDrawer.h>

#include <algorithm>
#include <nn/font/font_GpuBuffer.h>
#include <nn/gfx/gfx_BufferInfo.h>
#include <nn/gfx/gfx_MemoryPoolInfo.h>
#include <nn/gfx/gfx_StateInfo.h>
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_BitUtil.h>
#include <nn/gfx/gfx_GpuAddress.h>

#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/detail/gfx_ResShaderImpl.h>
#include <nn/gfx/gfx_Shader.h>
#include <nn/gfx/gfx_Device.h>

// The native embedded BNSH resource starts here and occupies 0x8428 bytes. Its original
// owning declaration and name are unknown; only its BinaryFileHeader is used by this query.
extern nn::util::BinaryFileHeader sUnk_7102524000;

namespace nn::font {

// NON_MATCHING: embedded-header addressing and state/arithmetic scheduling differ.
size_t RectDrawer::GetWorkBufferSize(gfx::Device* device, u32) {
    size_t vertex_state_size;
    {
        gfx::VertexStateInfo vertex_state;
        vertex_state.SetDefault();
        gfx::VertexAttributeStateInfo attribute;
        attribute.SetDefault();
        attribute.SetNamePtr("aVertex");
        attribute.SetBufferIndex(0);
        attribute.SetOffset(0);
        attribute.SetFormat(gfx::AttributeFormat_32_32_32_Float);
        gfx::VertexBufferStateInfo vertex_buffer;
        vertex_buffer.SetDefault();
        vertex_buffer.SetStride(28);
        vertex_state.SetVertexAttributeStateInfoArray(&attribute, 1);
        vertex_state.SetVertexBufferStateInfoArray(&vertex_buffer, 1);
        vertex_state_size =
            gfx::detail::VertexStateImpl<gfx::DefaultApi>::GetRequiredMemorySize(vertex_state);
    }

    size_t blend_state_size;
    {
        gfx::BlendStateInfo blend_state;
        blend_state.SetDefault();
        gfx::BlendTargetStateInfo blend_target;
        blend_target.SetDefault();
        blend_target.SetBlendEnabled(true);
        blend_target.SetSourceColorBlendFactor(gfx::BlendFactor_SourceAlpha);
        blend_target.SetDestinationColorBlendFactor(gfx::BlendFactor_OneMinusSourceAlpha);
        blend_target.SetColorBlendFunction(gfx::BlendFunction_Add);
        blend_target.SetChannelMask(gfx::ChannelMask_Red | gfx::ChannelMask_Green |
                                    gfx::ChannelMask_Blue);
        blend_state.SetBlendTargetStateInfoArray(&blend_target, 1);
        blend_state_size =
            gfx::detail::BlendStateImpl<gfx::DefaultApi>::GetRequiredMemorySize(blend_state);
    }

    gfx::MemoryPoolInfo pool_info;
    pool_info.SetDefault();
    const size_t pool_granularity =
        gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::GetPoolMemorySizeGranularity(device, pool_info);
    size_t index_pool_size;
    {
        gfx::BufferInfo index_info;
        index_info.SetDefault();
        index_info.SetGpuAccessFlags(gfx::GpuAccess_IndexBuffer);
        const size_t index_alignment = gfx::Buffer::GetBufferAlignment(device, index_info);
        const size_t index_size = util::align_up(size_t(112), index_alignment) + 12;
        index_pool_size = util::align_up(index_size, pool_granularity);
    }
    const size_t constant_pool_size = util::align_up(size_t(4), pool_granularity);
    const size_t shader_alignment = sUnk_7102524000.GetAlignment();
    const size_t pool_alignment =
        gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::GetPoolMemoryAlignment(device, pool_info);
    size_t constant_alignment;
    {
        gfx::BufferInfo constant_info;
        constant_info.SetDefault();
        constant_info.SetGpuAccessFlags(gfx::GpuAccess_ConstantBuffer);
        constant_alignment = gfx::Buffer::GetBufferAlignment(device, constant_info);
    }
    const size_t alignment = std::max(pool_alignment, constant_alignment);

    return util::align_up(vertex_state_size, 8) * 6 + util::align_up(blend_state_size, 8) +
           shader_alignment + util::align_up(index_pool_size, pool_granularity) +
           util::align_up(constant_pool_size, alignment) * 2 + 0x8428;
}

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
