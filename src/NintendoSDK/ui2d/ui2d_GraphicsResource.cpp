#include <nn/ui2d/GraphicsResource.h>

#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/gfx/gfx_StateInfo.h>
#include <nn/gfx/gfx_BufferInfo.h>
#include <nn/gfx/gfx_MemoryPoolInfo.h>
#include <nn/ui2d/Layout.h>
#include <nn/util/util_BitUtil.h>
#include <algorithm>
#include <new>

namespace nn::ui2d {

GraphicsResource::GraphicsResource() = default;

GraphicsResource::~GraphicsResource() = default;

// NON_MATCHING: alignment widths, payload store packing and info-object stack slots differ.
// 0x7100ac0c84
void GraphicsResource::InitializeVertexBuffer(gfx::Device* device, gfx::MemoryPool*, s64, u64) {
    gfx::BufferInfo vertex_alignment_info;
    vertex_alignment_info.SetDefault();
    vertex_alignment_info.SetGpuAccessFlags(gfx::GpuAccess_VertexBuffer);
    size_t vertex_alignment = gfx::Buffer::GetBufferAlignment(device, vertex_alignment_info);
    gfx::MemoryPoolInfo vertex_pool_alignment;
    vertex_pool_alignment.SetDefault();
    vertex_pool_alignment.SetMemoryPoolProperty(0x22);
    vertex_alignment = std::max(vertex_alignment,
        gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::GetPoolMemoryAlignment(
            device, vertex_pool_alignment));
    gfx::MemoryPoolInfo vertex_pool_granularity;
    vertex_pool_granularity.SetDefault();
    vertex_pool_granularity.SetMemoryPoolProperty(0x22);
    const size_t vertex_pool_size = util::align_up(size_t(32),
        gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::GetPoolMemorySizeGranularity(
            device, vertex_pool_granularity));
    mVertexData = new (Layout::AllocateMemory(vertex_pool_size, vertex_alignment)) util::Float2[4];
    mVertexData[0] = {{0.0f, 0.0f}};
    mVertexData[1] = {{1.0f, 0.0f}};
    mVertexData[2] = {{0.0f, 1.0f}};
    mVertexData[3] = {{1.0f, 1.0f}};

    gfx::BufferInfo index_alignment_info;
    index_alignment_info.SetDefault();
    index_alignment_info.SetGpuAccessFlags(gfx::GpuAccess_IndexBuffer);
    size_t index_alignment = gfx::Buffer::GetBufferAlignment(device, index_alignment_info);
    gfx::MemoryPoolInfo index_pool_alignment;
    index_pool_alignment.SetDefault();
    index_pool_alignment.SetMemoryPoolProperty(0x22);
    index_alignment = std::max(index_alignment,
        gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::GetPoolMemoryAlignment(
            device, index_pool_alignment));
    gfx::MemoryPoolInfo index_pool_granularity;
    index_pool_granularity.SetDefault();
    index_pool_granularity.SetMemoryPoolProperty(0x22);
    const size_t index_pool_size = util::align_up(size_t(12),
        gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::GetPoolMemorySizeGranularity(
            device, index_pool_granularity));
    mIndexData = new (Layout::AllocateMemory(index_pool_size, index_alignment)) u16[6];
    mIndexData[0] = 1;
    mIndexData[1] = 0;
    mIndexData[2] = 3;
    mIndexData[3] = 0;
    mIndexData[4] = 2;
    mIndexData[5] = 3;

    gfx::BufferInfo vertex_info;
    vertex_info.SetDefault();
    vertex_info.SetSize(32);
    vertex_info.SetGpuAccessFlags(gfx::GpuAccess_VertexBuffer);
    gfx::MemoryPoolInfo vertex_pool;
    vertex_pool.SetDefault();
    vertex_pool.SetMemoryPoolProperty(0x22);
    vertex_pool.SetPoolMemory(mVertexData, vertex_pool_size);
    mMemoryPool0.gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::Initialize(device, vertex_pool);
    mBuffer0.gfx::detail::BufferImpl<gfx::DefaultApi>::Initialize(
        device, vertex_info, &mMemoryPool0, 0, 32);

    gfx::BufferInfo index_info;
    index_info.SetDefault();
    index_info.SetSize(12);
    index_info.SetGpuAccessFlags(gfx::GpuAccess_IndexBuffer);
    gfx::MemoryPoolInfo index_pool;
    index_pool.SetDefault();
    index_pool.SetMemoryPoolProperty(0x22);
    index_pool.SetPoolMemory(mIndexData, index_pool_size);
    mMemoryPool1.gfx::detail::MemoryPoolImpl<gfx::DefaultApi>::Initialize(device, index_pool);
    mBuffer1.gfx::detail::BufferImpl<gfx::DefaultApi>::Initialize(
        device, index_info, &mMemoryPool1, 0, 12);
}

// Native readonly resource-code conversion tables at 0x7101e7f6b0 and 0x7101e7f6f0.
const gfx::LogicOperation sUnk_7101E7F6B0[16] = {
    gfx::LogicOperation_NoOp, gfx::LogicOperation_Clear, gfx::LogicOperation_Set,
    gfx::LogicOperation_Copy, gfx::LogicOperation_CopyInverted, gfx::LogicOperation_Invert,
    gfx::LogicOperation_And, gfx::LogicOperation_Nand, gfx::LogicOperation_Or,
    gfx::LogicOperation_Nor, gfx::LogicOperation_Xor, gfx::LogicOperation_Equiv,
    gfx::LogicOperation_AndReverse, gfx::LogicOperation_AndInverted,
    gfx::LogicOperation_OrReverse, gfx::LogicOperation_OrInverted,
};
const gfx::BlendFactor sUnk_7101E7F6F0[10] = {
    gfx::BlendFactor_Zero, gfx::BlendFactor_One,
    gfx::BlendFactor_DestinationColor, gfx::BlendFactor_OneMinusDestinationColor,
    gfx::BlendFactor_SourceAlpha, gfx::BlendFactor_OneMinusSourceAlpha,
    gfx::BlendFactor_DestinationAlpha, gfx::BlendFactor_OneMinusDestinationAlpha,
    gfx::BlendFactor_SourceColor, gfx::BlendFactor_OneMinusSourceColor,
};

// NON_MATCHING: logic-table index extension and the shared result store differ.
// 0x7100ac0fd0
size_t GraphicsResource::SetupBlendStateInfo(gfx::BlendStateInfo* state,
                                            gfx::BlendTargetStateInfo* target,
                                            const ResBlendMode* color,
                                            const ResBlendMode* alpha) {
    state->SetDefault();
    target->SetDefault();
    state->SetBlendTargetStateInfoArray(target, 1);
    if (color->logicOp) {
        state->SetLogicOperationEnabled(true);
        target->SetBlendEnabled(false);
        const u32 operation = color->logicOp - 1;
        state->SetLogicOperation(operation < 16 ? sUnk_7101E7F6B0[operation] :
                                                gfx::LogicOperation_Clear);
    } else if (color->blendOp) {
        state->SetLogicOperationEnabled(false);
        target->SetBlendEnabled(true);
        target->SetSourceColorBlendFactor(color->srcFactor < 10 ?
            sUnk_7101E7F6F0[color->srcFactor] : gfx::BlendFactor_Zero);
        target->SetDestinationColorBlendFactor(color->dstFactor < 10 ?
            sUnk_7101E7F6F0[color->dstFactor] : gfx::BlendFactor_Zero);
        target->SetSourceAlphaBlendFactor(alpha->srcFactor < 10 ?
            sUnk_7101E7F6F0[alpha->srcFactor] : gfx::BlendFactor_Zero);
        target->SetDestinationAlphaBlendFactor(alpha->dstFactor < 10 ?
            sUnk_7101E7F6F0[alpha->dstFactor] : gfx::BlendFactor_Zero);
        const u32 color_operation = color->blendOp - 1;
        target->SetColorBlendFunction(color_operation < 5 ?
            static_cast<gfx::BlendFunction>(color_operation) : gfx::BlendFunction_Add);
        const u32 alpha_operation = alpha->blendOp - 1;
        target->SetAlphaBlendFunction(alpha_operation < 5 ?
            static_cast<gfx::BlendFunction>(alpha_operation) : gfx::BlendFunction_Add);
    } else {
        target->SetBlendEnabled(false);
    }
    return gfx::detail::BlendStateImpl<gfx::ApiVariationNvn8>::GetRequiredMemorySize(*state);
}

int GraphicsResource::GetPresetBlendStateId(const ResBlendMode* color, const ResBlendMode* alpha) {
    const ResBlendMode* colorPresets;
    const ResBlendMode* alphaPresets;
    GetPresetBlendModeArray(&colorPresets, &alphaPresets);
    for (int i = 0; i < 6; ++i) {
        if (colorPresets[i].blendOp == color->blendOp &&
            colorPresets[i].srcFactor == color->srcFactor &&
            colorPresets[i].dstFactor == color->dstFactor &&
            colorPresets[i].logicOp == color->logicOp &&
            alphaPresets[i].blendOp == alpha->blendOp &&
            alphaPresets[i].srcFactor == alpha->srcFactor &&
            alphaPresets[i].dstFactor == alpha->dstFactor &&
            alphaPresets[i].logicOp == alpha->logicOp)
            return i;
    }
    return 7;
}

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
gfx::DescriptorSlot* GraphicsResource::sub_7100AC1B30(TexWrap wrap_s, TexWrap wrap_t,
                                                   TexFilter min_filter, TexFilter mag_filter) {
    const int index = wrap_s * 12 + wrap_t * 4 + min_filter + mag_filter * 2;
    return &_9b8[index];
}

}  // namespace nn::ui2d
