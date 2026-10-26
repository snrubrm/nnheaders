#include <nn/vfx/Primitive.h>

#include <nn/gfx/gfx_GpuAddress.h>

namespace nn::vfx {

u32 Primitive::GetVertexBufferArrayCount() const {
    return mVertexBufferArrayCount;
}

u32 Primitive::GetIndexCount() const {
    return mIndexCount;
}

u64 Primitive::GetStrideSize(int) const {
    return mStrideSize;
}

u64 Primitive::GetVertexBufferSize(int) const {
    return mVertexBufferSize;
}

void Primitive::GetVertexBufferGpuAddress(gfx::GpuAddress* address, int) const {
    *address = mVertexBufferGpuAddress;
}

const gfx::GpuAddress& Primitive::GetIndexBufferGpuAddress() const {
    return mIndexBufferGpuAddress;
}

u32 Primitive::GetBufferCount() const {
    return 1;
}

u32 Primitive::GetBufferIndex(PrimitiveDataElementIndex) const {
    return 0;
}

gfx::AttributeFormat Primitive::GetFormat(PrimitiveDataElementIndex) const {
    return gfx::AttributeFormat_32_32_32_32_Float;
}

gfx::IndexFormat Primitive::GetIndexFormat() const {
    return gfx::IndexFormat_Uint32;
}

u64 Primitive::GetOffset(PrimitiveDataElementIndex index) const {
    return mOffsets[index];
}

u32 G3dPrimitive::GetVertexBufferArrayCount() const {
    return mShape->mVertexBufferArrayCount;
}

u32 G3dPrimitive::GetIndexCount() const {
    return mIndex->mIndexCount;
}

s64 G3dPrimitive::GetStrideSize(int index) const {
    return mShape->mStrideSizes[index].value;
}

u32 G3dPrimitive::GetVertexBufferSize(int index) const {
    return mShape->mVertexBufferSizes[index].value;
}

void G3dPrimitive::GetVertexBufferGpuAddress(gfx::GpuAddress* address, int index) const {
    mShape->mBuffers[index]->GetGpuAddress(address);
}

const gfx::GpuAddress& G3dPrimitive::GetIndexBufferGpuAddress() const {
    return mIndexBufferGpuAddress;
}

u32 G3dPrimitive::GetBufferCount() const {
    return mShape->mBufferCount;
}

u32 G3dPrimitive::GetBufferIndex(PrimitiveDataElementIndex index) const {
    return mBufferIndices[index];
}

gfx::AttributeFormat G3dPrimitive::GetFormat(PrimitiveDataElementIndex index) const {
    return mFormats[index];
}

gfx::IndexFormat G3dPrimitive::GetIndexFormat() const {
    return mIndex->mFormat;
}

u64 G3dPrimitive::GetOffset(PrimitiveDataElementIndex index) const {
    return mOffsets[index];
}

}  // namespace nn::vfx
