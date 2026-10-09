#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/gfx/detail/gfx_State-api.nvn.8.h>

namespace nn::gfx {
class ResShaderFile;
}

namespace nn::ui2d {

// ShaderContainer::RegisterShader returns entry +0x18; its next flag is at +0x58.
class ShaderInfo {
public:
    void Initialize(gfx::Device* device, void* shader_file);
    void Initialize(gfx::Device* device, void* shader_file, const void* archive_data,
                    gfx::MemoryPool* memory_pool, ptrdiff_t memory_pool_offset, size_t memory_pool_size);
    void Finalize(gfx::Device* device, bool keep_shader_resource);
    void sub_7100AC5710(gfx::CommandBuffer* command_buffer, int variation);

    /* 0x00 */ gfx::ResShaderFile* mResShaderFile = nullptr;
    /* 0x08 */ int mShaderSelection = 2;
    /* 0x0c */ u8 _c[4];
    /* 0x10 */ gfx::detail::VertexStateImpl<gfx::DefaultApi>* mVertexStates = nullptr;
    /* 0x18 */ int* mVertexConstantBufferSlots = nullptr;
    /* 0x20 */ int* mGeometryConstantBufferSlots = nullptr;
    /* 0x28 */ int* mPixelConstantBufferSlots = nullptr;
    /* 0x30 */ int* mTextureSlots = nullptr;
    /* 0x38 */ const void* _38 = nullptr;
};
static_assert(sizeof(ShaderInfo) == 0x40);

}  // namespace nn::ui2d
