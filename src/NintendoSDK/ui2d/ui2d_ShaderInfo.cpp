#include <nn/ui2d/ShaderInfo.h>

#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/gfx_ResShaderData-api.nvn.h>
#include <nn/gfx/detail/gfx_ResShaderImpl.h>
#include <nn/gfx/detail/gfx_Shader-api.nvn.8.h>
#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#include <nn/gfx/gfx_StateInfo.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/ui2d/Layout.h>
#include <nn/util/util_BytePtr.h>
#include <new>

namespace nn::ui2d {

void ShaderInfo::Initialize(gfx::Device* device, void* shader_file) {
    Initialize(device, shader_file, nullptr, nullptr, 0, 0);
}

// NON_MATCHING: shader metadata uses one resource lookup; allocation and argument scheduling differ.
void ShaderInfo::Initialize(gfx::Device* device, void* shader_file, const void* archive_data,
                            gfx::MemoryPool* memory_pool, ptrdiff_t memory_pool_offset,
                            size_t memory_pool_size) {
    mResShaderFile = gfx::ResShaderFile::ResCast(shader_file);
    auto* container = mResShaderFile->GetShaderContainer();
    auto* pool = static_cast<gfx::NvnShaderPool*>(container->ToData().pShaderBinaryPool.Get());
    auto* internal_pool = pool ? static_cast<gfx::detail::MemoryPoolImpl<gfx::DefaultApi>*>(
                                    pool->pMemoryPool.Get()) : nullptr;
    if (!internal_pool || internal_pool->ToData()->state != 1) {
        if (memory_pool) {
            gfx::detail::ResShaderContainerImpl::Initialize<gfx::DefaultApi>(
                container, device, memory_pool,
                memory_pool_offset + util::BytePtr(shader_file).Distance(container), memory_pool_size);
        } else {
            gfx::detail::ResShaderContainerImpl::Initialize<gfx::DefaultApi>(
                container, device, nullptr, 0, 0);
        }
    }
    auto* program = container->GetResShaderVariation(0)->GetResShaderProgram(gfx::ShaderCodeType_Binary);
    mShaderSelection = gfx::ShaderCodeType_Binary;
    if (!program ||
        (!static_cast<gfx::detail::ShaderImpl<gfx::DefaultApi>*>(program->GetShader())->ToData()->state &&
         static_cast<gfx::detail::ShaderImpl<gfx::DefaultApi>*>(program->GetShader())->Initialize(
             device, *program->GetShaderInfo()) != gfx::ShaderInitializeResult_Success)) {
        mShaderSelection = gfx::ShaderCodeType_Source;
        program = container->GetResShaderVariation(0)->GetResShaderProgram(gfx::ShaderCodeType_Source);
        gfx::detail::ShaderImpl<gfx::DefaultApi>* shader = program->GetShader();
        if (!shader->ToData()->state)
            shader->Initialize(device, *program->GetShaderInfo());
    }
    for (int i = 1, count = mResShaderFile->GetShaderContainer()->GetShaderVariationCount(); i < count; ++i) {
        auto* variation_program = container->GetResShaderVariation(i)->GetResShaderProgram(
            static_cast<gfx::ShaderCodeType>(mShaderSelection));
        gfx::detail::ShaderImpl<gfx::DefaultApi>* shader = variation_program->GetShader();
        if (!shader->ToData()->state)
            shader->Initialize(device, *variation_program->GetShaderInfo());
    }
    const int count = mResShaderFile->GetShaderContainer()->GetShaderVariationCount();
    auto* vertex_states = static_cast<gfx::detail::VertexStateImpl<gfx::DefaultApi>*>(
        Layout::AllocateMemory(sizeof(*mVertexStates) * count, 4));
    if (vertex_states) {
        for (int i = 0; i < count; ++i)
            new (&vertex_states[i]) gfx::detail::VertexStateImpl<gfx::DefaultApi>;
    }
    mVertexStates = vertex_states;
    mVertexConstantBufferSlots = static_cast<int*>(Layout::AllocateMemory(sizeof(int) * count, 4));
    mGeometryConstantBufferSlots = static_cast<int*>(Layout::AllocateMemory(sizeof(int) * count, 4));
    mPixelConstantBufferSlots = static_cast<int*>(Layout::AllocateMemory(sizeof(int) * count, 4));
    mTextureSlots = static_cast<int*>(Layout::AllocateMemory(sizeof(int) * count * 3, 4));
    for (int i = 0, variation_count = mResShaderFile->GetShaderContainer()->GetShaderVariationCount();
         i < variation_count; ++i) {
        gfx::detail::ShaderImpl<gfx::DefaultApi>* shader =
            mResShaderFile->GetShaderContainer()->GetResShaderVariation(i)
                ->GetResShaderProgram(static_cast<gfx::ShaderCodeType>(mShaderSelection))->GetShader();
        mVertexConstantBufferSlots[i] = shader->GetInterfaceSlot(
            gfx::ShaderStage_Vertex, gfx::ShaderInterfaceType_ConstantBuffer, "uConstantBufferForVertexShader");
        mGeometryConstantBufferSlots[i] = shader->GetInterfaceSlot(
            gfx::ShaderStage_Geometry, gfx::ShaderInterfaceType_ConstantBuffer, "uConstantBufferForGeometryShader");
        mPixelConstantBufferSlots[i] = shader->GetInterfaceSlot(
            gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_ConstantBuffer, "uConstantBufferForPixelShader");
        mTextureSlots[i * 3] = shader->GetInterfaceSlot(
            gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_Sampler, "uTexture0");
        mTextureSlots[i * 3 + 1] = shader->GetInterfaceSlot(
            gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_Sampler, "uTexture1");
        mTextureSlots[i * 3 + 2] = shader->GetInterfaceSlot(
            gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_Sampler, "uTexture2");
    }
    gfx::VertexStateInfo state_info;
    state_info.SetDefault();
    gfx::VertexAttributeStateInfo attribute_info;
    attribute_info.SetDefault();
    attribute_info.SetNamePtr("aVertexIndex");
    attribute_info.SetBufferIndex(0);
    attribute_info.SetOffset(0);
    attribute_info.SetFormat(gfx::AttributeFormat_32_32_Float);
    gfx::VertexBufferStateInfo buffer_info;
    buffer_info.SetDefault();
    buffer_info.SetStride(8);
    buffer_info.SetDivisor(0);
    state_info.SetVertexAttributeStateInfoArray(&attribute_info, 1);
    state_info.SetVertexBufferStateInfoArray(&buffer_info, 1);
    const size_t memory_size = gfx::detail::VertexStateImpl<gfx::DefaultApi>::GetRequiredMemorySize(state_info);
    for (int i = 0, variation_count = mResShaderFile->GetShaderContainer()->GetShaderVariationCount();
         i < variation_count; ++i) {
        mVertexStates[i].SetMemory(Layout::AllocateMemory(memory_size, 8), memory_size);
        gfx::detail::ShaderImpl<gfx::DefaultApi>* shader =
            mResShaderFile->GetShaderContainer()->GetResShaderVariation(i)
                ->GetResShaderProgram(static_cast<gfx::ShaderCodeType>(mShaderSelection))->GetShader();
        mVertexStates[i].Initialize(device, state_info, shader);
    }
    _38 = archive_data;
}

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
