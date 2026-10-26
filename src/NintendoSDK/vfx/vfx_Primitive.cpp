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

}  // namespace nn::vfx
