#include <nn/ui2d/GraphicsResource.h>

#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/gfx/gfx_StateInfo.h>

namespace nn::ui2d {

GraphicsResource::GraphicsResource() = default;

GraphicsResource::~GraphicsResource() = default;

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
