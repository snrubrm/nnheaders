#include <nn/ui2d/Material.h>
#include <nn/ui2d/AnimTransform.h>
#include <nn/ui2d/BuildTypes.h>
#include <nn/font/font_GpuBuffer.h>
#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/ui2d/DrawInfo.h>
#include <nn/ui2d/GraphicsResource.h>
#include <nn/ui2d/ShaderInfo.h>
#include <nn/util/util_BytePtr.h>

namespace nn::ui2d {

// 0x7100ac35cc
// NON_MATCHING: the compiler removes the unused native destructor vtable store.
Material::~Material() {}

namespace detail {
// 0x7100ac01fc
const ResMaterial* GetResMaterial(const BuildResSet* resources, u16 index) {
    const u32* offsets = util::ConstBytePtr(resources->mMaterialList, 0xc).Get<u32>();
    return util::ConstBytePtr(resources->mMaterialList, offsets[index]).Get<ResMaterial>();
}
}  // namespace detail

// 0x7100aba544
TexMap::TexMap() : mTextureInfo(nullptr) {
    mFlags = 0x90;
}

// 0x7100aba554
TexMap::TexMap(const TextureInfo* info) {
    Set(info);
}

// 0x7100aba57c
TexMap::~TexMap() {}

// 0x7100aba580
void TexMap::Finalize() {}

// 0x7100aba568
void TexMap::Set(const TextureInfo* info) {
    if (info) {
        mTextureInfo = info;
        mFlags = 0x90;
    }
}

// 0x7100aba584
void TexMap::SetWrapMode(TexWrap wrap_s, TexWrap wrap_t) {
    mWrapS = wrap_s;
    mWrapT = wrap_t;
}

// 0x7100aba5a0
void TexMap::SetFilter(TexFilter min_filter, TexFilter mag_filter) {
    mMinFilter = min_filter;
    mMagFilter = mag_filter;
}

// 0x7100ac3720
void Material::BindAnimation(AnimTransform* transform) {
    transform->BindMaterial(this);
}

// 0x7100ac3738
void Material::UnbindAnimation(AnimTransform* transform) {
    transform->UnbindMaterial(this);
}

// 0x7100ac391c
void* Material::GetConstantBufferForVertexShader(const DrawInfo& info) const {
    void* mapped = info.mUi2dConstantBuffer->mMappedPtr;
    if (!mapped)
        return nullptr;
    return util::BytePtr(mapped, mVertexShaderConstantBufferOffset).Get();
}

// 0x7100ac4ab0
void* Material::GetConstantBufferForPixelShader(const DrawInfo& info) const {
    void* mapped = info.mUi2dConstantBuffer->mMappedPtr;
    if (!mapped)
        return nullptr;
    return util::BytePtr(mapped, mPixelShaderConstantBufferOffset).Get();
}

// Shared allocation uses native acquire/release exclusive instructions at 0x7100ac4b1c,
// 0x7100ac4b80 and 0x7100ac4bd0; the other union member is a plain byte offset.
// 0x7100ac4ad0
// NON_MATCHING: alignment and allocation result scheduling differ from native code.
void Material::sub_7100AC4AD0(const DrawInfo& info) {
    const size_t alignment = info.mGraphicsResource->_70;
    size_t size = 560;
    if (mUserShaderConstantBufferInformation)
        size += mUserShaderConstantBufferInformation->vertexExtraSize;
    size = (size + alignment - 1) & ~(alignment - 1);
    if (info.mUi2dConstantBuffer->mFlags & 1) {
        mVertexShaderConstantBufferOffset =
            info.mUi2dConstantBuffer->mSharedOffset->fetch_add(size, std::memory_order_acq_rel);
    } else {
        u64 offset = info.mUi2dConstantBuffer->mOffset;
        info.mUi2dConstantBuffer->mOffset += size;
        mVertexShaderConstantBufferOffset = offset;
    }

    size = 144;
    if (mMemCap._bit16)
        size = 224;
    else if (mUserShaderConstantBufferInformation)
        size += mUserShaderConstantBufferInformation->pixelExtraSize;
    size = (size + alignment - 1) & ~(alignment - 1);
    if (info.mUi2dConstantBuffer->mFlags & 1) {
        mPixelShaderConstantBufferOffset =
            info.mUi2dConstantBuffer->mSharedOffset->fetch_add(size, std::memory_order_acq_rel);
    } else {
        u64 offset = info.mUi2dConstantBuffer->mOffset;
        info.mUi2dConstantBuffer->mOffset += size;
        mPixelShaderConstantBufferOffset = offset;
    }

    if (mUserShaderConstantBufferInformation && mUserShaderConstantBufferInformation->geometrySize) {
        size = mUserShaderConstantBufferInformation->geometrySize;
        size = (size + alignment - 1) & ~(alignment - 1);
        if (info.mUi2dConstantBuffer->mFlags & 1) {
            mUserShaderConstantBufferInformation->geometryOffset =
                info.mUi2dConstantBuffer->mSharedOffset->fetch_add(size, std::memory_order_acq_rel);
        } else {
            u64 offset = info.mUi2dConstantBuffer->mOffset;
            info.mUi2dConstantBuffer->mOffset += size;
            mUserShaderConstantBufferInformation->geometryOffset = offset;
        }
    }
}

// 0x7100ac4e54
void Material::sub_7100AC4E54(gfx::CommandBuffer* command_buffer, const DrawInfo& info) {
    gfx::GpuAddress address;
    info.mUi2dConstantBuffer->mBuffers[info.mUi2dConstantBuffer->mBufferIndex].GetGpuAddress(&address);
    address.Offset(mVertexShaderConstantBufferOffset);
    size_t size = 560;
    if (mUserShaderConstantBufferInformation)
        size += mUserShaderConstantBufferInformation->vertexExtraSize;
    command_buffer->gfx::detail::CommandBufferImpl<gfx::DefaultApi>::SetConstantBuffer(
        mShaderInfo->mVertexConstantBufferSlots[_4b], gfx::ShaderStage_Vertex, address, size);
}

// 0x7100ac4ee4
void Material::sub_7100AC4EE4(gfx::CommandBuffer* command_buffer, const DrawInfo& info) {
    if (!mUserShaderConstantBufferInformation || !mUserShaderConstantBufferInformation->geometryOffset)
        return;
    gfx::GpuAddress address;
    info.mUi2dConstantBuffer->mBuffers[info.mUi2dConstantBuffer->mBufferIndex].GetGpuAddress(&address);
    address.Offset(mUserShaderConstantBufferInformation->geometryOffset);
    command_buffer->gfx::detail::CommandBufferImpl<gfx::DefaultApi>::SetConstantBuffer(
        mShaderInfo->mGeometryConstantBufferSlots[_4b], gfx::ShaderStage_Geometry, address,
        mUserShaderConstantBufferInformation->geometrySize);
}

// 0x7100ac4f74
void Material::sub_7100AC4F74(gfx::CommandBuffer* command_buffer, const DrawInfo& info) {
    gfx::GpuAddress address;
    info.mUi2dConstantBuffer->mBuffers[info.mUi2dConstantBuffer->mBufferIndex].GetGpuAddress(&address);
    address.Offset(mPixelShaderConstantBufferOffset);
    size_t size = 144;
    if (mUserShaderConstantBufferInformation)
        size += mUserShaderConstantBufferInformation->pixelExtraSize;
    command_buffer->gfx::detail::CommandBufferImpl<gfx::DefaultApi>::SetConstantBuffer(
        mShaderInfo->mPixelConstantBufferSlots[_4b], gfx::ShaderStage_Pixel, address, size);
}

// 0x7100ac50cc
void Material::sub_7100AC50CC(gfx::CommandBuffer* command_buffer) {
    mShaderInfo->sub_7100AC5710(command_buffer, _4b);
}

}  // namespace nn::ui2d
