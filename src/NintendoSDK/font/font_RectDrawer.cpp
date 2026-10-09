#include <nn/font/font_RectDrawer.h>

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

void RectDrawer::RegisterSamplerSlot(SamplerRegistrationCallback callback, void* user_data) {
    callback(&mSamplerSlot, mSampler, user_data);
}

void RectDrawer::UnregisterSamplerSlot(SamplerDescriptorCallback callback, void* user_data) {
    callback(&mSamplerSlot, mSampler, user_data);
    mSamplerSlot.Invalidate();
}

}  // namespace nn::font
