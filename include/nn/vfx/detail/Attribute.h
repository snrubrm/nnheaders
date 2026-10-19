#pragma once

#include <nn/types.h>

namespace nn::vfx {

// Selects one of the two buffers an Attribute owns (the constants use 0 and 1).
enum class BufferSide : u32;

namespace detail {

class BufferAllocator;

// The vertex and index buffers of a vfx draw path. The constructor 0x7100ad381c
// only clears the two buffer records at +0x20 and the two mapped pointers at
// +0x40; Initialize 0x7100ad382c fills them through the allocator.
class Attribute {
public:
    Attribute();
    void Initialize(BufferAllocator* allocator, int count, size_t size);
    void Finalize();
    // 0x7100ad3988: the mapped pointer of the side.
    void* Map(BufferSide side);
    // 0x7100ad3994: nothing to do; the original body is empty.
    void Unmap();
    // 0x7100ad3998: the address of the side's buffer record.
    const void* GetGpuAddress(BufferSide side) const;

private:
    struct Buffer {
        u8 _0[0x10];
    };

    void* _0;
    u8 _8[8];
    BufferAllocator* mAllocator;
    u8 _18[8];
    Buffer mBuffers[2] = {};
    void* mMapped[2] = {};
};
static_assert(sizeof(Attribute) == 0x50);

}  // namespace detail
}  // namespace nn::vfx
