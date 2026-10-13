#pragma once

#include <nn/vfx/Heap.h>

namespace nn::vfx::detail {

// Native RTTI supplies the class name. Complete System and Resource constructors
// place this 0x20-byte adapter at +0x40 and +0x450 respectively.
class CalculateAllocSizeHeap : public Heap {
public:
    CalculateAllocSizeHeap();
    ~CalculateAllocSizeHeap() override = default;

    void* Alloc(size_t size, size_t alignment) override;
    void Free(void* ptr) override;

private:
    // Member names describe the complete native allocation/free operations.
    Heap* mHeap;
    size_t mAllocatedSize;
    s32 mAllocationCount;
};
static_assert(sizeof(CalculateAllocSizeHeap) == 0x20);

}  // namespace nn::vfx::detail
