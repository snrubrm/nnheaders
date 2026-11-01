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
    void* Alloc(size_t size, void (*dispose)(void*, size_t, void*), void* dispose_arg,
                void (*callback)(void*), void* callback_arg);

private:
    void* mHeap;
    void* mNext;
    void* mPrev;
};
static_assert(sizeof(FrameHeap) == 0x18);

}  // namespace nn::atk::detail
