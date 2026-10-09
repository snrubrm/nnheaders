#include <nn/ui2d/ShaderInfo.h>

#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/gfx_ResShaderData-api.nvn.h>
#include <nn/gfx/detail/gfx_ResShaderImpl.h>
#include <nn/gfx/detail/gfx_Shader-api.nvn.8.h>
#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#include <nn/ui2d/Layout.h>

namespace nn::ui2d {

void ShaderInfo::sub_7100AC5710(gfx::CommandBuffer* command_buffer, int variation) {
    gfx::detail::ShaderImpl<gfx::DefaultApi>* shader =
        mResShaderFile->GetShaderContainer()->GetResShaderVariation(variation)
            ->GetResShaderProgram(static_cast<gfx::ShaderCodeType>(mShaderSelection))->GetShader();
    command_buffer->gfx::detail::CommandBufferImpl<gfx::DefaultApi>::SetShader(shader, 0x3f);
    command_buffer->gfx::detail::CommandBufferImpl<gfx::DefaultApi>::SetVertexState(
        &mVertexStates[variation]);
}

// NON_MATCHING: variation and vertex-state loop offsets use different registers.
void ShaderInfo::Finalize(gfx::Device* device, bool keep_shader_resource) {
    const int count = mResShaderFile->GetShaderContainer()->GetShaderVariationCount();
    for (int i = 0; i < count; ++i) {
        void* memory = mVertexStates[i].GetMemory();
        mVertexStates[i].Finalize(device);
        Layout::FreeMemory(memory);
        if (!keep_shader_resource) {
            auto* program = mResShaderFile->GetShaderContainer()->GetResShaderVariation(i)
                                ->GetResShaderProgram(
                                    static_cast<gfx::ShaderCodeType>(mShaderSelection));
            if (program) {
                gfx::detail::ShaderImpl<gfx::DefaultApi>* shader = program->GetShader();
                if (shader->ToData()->state)
                    shader->Finalize(device);
            }
        }
    }
    if (!keep_shader_resource) {
        auto* container = mResShaderFile->GetShaderContainer();
        auto* pool = static_cast<gfx::NvnShaderPool*>(container->ToData().pShaderBinaryPool.Get());
        if (pool) {
            auto* memory_pool = static_cast<gfx::detail::MemoryPoolImpl<gfx::DefaultApi>*>(
                pool->pMemoryPool.Get());
            if (memory_pool && memory_pool->ToData()->state == 1)
                gfx::detail::ResShaderContainerImpl::Finalize<gfx::DefaultApi>(container, device);
        }
    }
    auto* vertex_states = mVertexStates;
    const int vertex_count = mResShaderFile->GetShaderContainer()->GetShaderVariationCount();
    if (vertex_states) {
        for (int i = 0; i < vertex_count; ++i)
            vertex_states[i].~VertexStateImpl();
        Layout::FreeMemory(vertex_states);
    }
    Layout::FreeMemory(mVertexConstantBufferSlots);
    if (mGeometryConstantBufferSlots)
        Layout::FreeMemory(mGeometryConstantBufferSlots);
    Layout::FreeMemory(mPixelConstantBufferSlots);
    Layout::FreeMemory(mTextureSlots);
}

}  // namespace nn::ui2d
