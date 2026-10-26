#pragma once

#include <nn/gfx/gfx_Buffer.h>
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

// TODO: incomplete. The same query interface over a G3d shape's GPU buffers. The shape and index
// records are opaque here; their first members are the fields the getters below read.
class G3dPrimitive : public IPrimitive {
public:
    u32 GetVertexBufferArrayCount() const;
    u32 GetIndexCount() const;
    s64 GetStrideSize(int index) const;
    u32 GetVertexBufferSize(int index) const;
    void GetVertexBufferGpuAddress(gfx::GpuAddress*, int index) const;
    const gfx::GpuAddress& GetIndexBufferGpuAddress() const;
    u32 GetBufferCount() const;
    u32 GetBufferIndex(PrimitiveDataElementIndex) const;
    gfx::AttributeFormat GetFormat(PrimitiveDataElementIndex) const;
    gfx::IndexFormat GetIndexFormat() const;
    u64 GetOffset(PrimitiveDataElementIndex) const;

private:
    struct VertexBufferSize {
        u32 value;
        u8 _4[0xc];
    };
    struct VertexBufferStride {
        s32 value;
        u8 _4[0xc];
    };
    struct ShapeRecord {
        u8 _0[0x30];
        /* 0x30 */ const gfx::TBuffer<gfx::ApiVariationNvn8>* const* mBuffers;
        /* 0x38 */ const VertexBufferSize* mVertexBufferSizes;
        /* 0x40 */ const VertexBufferStride* mStrideSizes;
        u8 _48[0xd];
        /* 0x55 */ u8 mBufferCount;
        u8 _56[2];
        /* 0x58 */ u32 mVertexBufferArrayCount;
    };
    struct IndexRecord {
        u8 _0[0x28];
        /* 0x28 */ gfx::IndexFormat mFormat;
        /* 0x2c */ u32 mIndexCount;
    };

    u8 _0[0x18];
    /* 0x18 */ const ShapeRecord* mShape;
    u8 _20[8];
    /* 0x28 */ const IndexRecord* mIndex;
    /* 0x30 */ gfx::GpuAddress mIndexBufferGpuAddress;
    /* 0x40 */ gfx::AttributeFormat mFormats[2];
    u8 _48[0xc];
    /* 0x54 */ u32 mBufferIndices[2];
    u8 _5c[0xc];
    /* 0x68 */ u64 mOffsets[2];
};

}  // namespace nn::vfx
