#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// SoundHeap owns this object at +0x28. Its constructor initializes a heap pointer and a
// circular pair of links; the pointed-to objects remain opaque.
class FrameHeap {
public:
    FrameHeap();
    ~FrameHeap();
    bool Create(void* address, size_t size);
    // Return types are inferred from callers that discard the results.
    void Clear();
    void Destroy();
    // Inline-only in the original; name is a guess. Create, Destroy, and the destructor
    // each test this heap pointer before operating on it.
    bool IsCreated() const { return mHeap != nullptr; }
    void* Alloc(size_t size, void (*dispose)(void*, size_t, void*), void* dispose_arg,
                void (*callback)(void*), void* callback_arg);

private:
    void* mHeap;
    void* mNext;
    void* mPrev;
};
static_assert(sizeof(FrameHeap) == 0x18);

}  // namespace nn::atk::detail
