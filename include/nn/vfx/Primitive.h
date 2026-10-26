#pragma once

#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/types.h>

namespace nn::vfx {

class IPrimitive {
public:
    enum PrimitiveDataElementIndex {
        PrimitiveDataElementIndex_0,
        PrimitiveDataElementIndex_1,
    };
};

// TODO: incomplete. Only the vertex/index buffer queries are modelled; one buffer holds the two
// data elements.
class Primitive : public IPrimitive {
public:
    u32 GetVertexBufferArrayCount() const;
    u32 GetIndexCount() const;
    u64 GetStrideSize(int) const;
    u64 GetVertexBufferSize(int) const;
    void GetVertexBufferGpuAddress(gfx::GpuAddress*, int) const;
    const gfx::GpuAddress& GetIndexBufferGpuAddress() const;
    u32 GetBufferCount() const;
    u32 GetBufferIndex(PrimitiveDataElementIndex) const;
    gfx::AttributeFormat GetFormat(PrimitiveDataElementIndex) const;
    gfx::IndexFormat GetIndexFormat() const;
    u64 GetOffset(PrimitiveDataElementIndex) const;

private:
    u8 _0[0x10];
    /* 0x10 */ u32 mVertexBufferArrayCount;
    /* 0x14 */ u32 mIndexCount;
    u8 _18[8];
    /* 0x20 */ u64 mStrideSize;
    /* 0x28 */ u64 mVertexBufferSize;
    u8 _30[8];
    /* 0x38 */ gfx::GpuAddress mVertexBufferGpuAddress;
    /* 0x48 */ gfx::GpuAddress mIndexBufferGpuAddress;
    u8 _58[0x18];
    /* 0x70 */ u64 mOffsets[2];
};

}  // namespace nn::vfx
