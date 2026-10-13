#include <nn/vfx/detail/CalculateAllocSizeHeap.h>

namespace nn::vfx::detail {

void* CalculateAllocSizeHeap::Alloc(size_t size, size_t alignment) {
    void* ptr = mHeap->Alloc(size, alignment);
    mAllocatedSize += size;
    ++mAllocationCount;
    return ptr;
}

void CalculateAllocSizeHeap::Free(void* ptr) {
    --mAllocationCount;
    mHeap->Free(ptr);
}

}  // namespace nn::vfx::detail
