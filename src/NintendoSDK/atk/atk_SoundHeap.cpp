#include <nn/atk/SoundHeap.h>
#include <nn/atk/detail/Util.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk {

SoundHeap::SoundHeap() : mMutex(true) {}

bool SoundHeap::Create(void* address, size_t size) {
    detail::Util::IsValidMemoryForDsp(address, size);
    mMutex.Lock();
    bool result = mFrameHeap.Create(address, size);
    mMutex.Unlock();
    return result;
}

void* SoundHeap::Allocate(size_t size) {
    mMutex.Lock();
    void* result = mFrameHeap.Alloc(size, DisposeCallbackFunc, nullptr, nullptr, nullptr);
    mMutex.Unlock();
    return result;
}

void* SoundHeap::Allocate(size_t size, void (*callback)(void*), void* arg) {
    mMutex.Lock();
    void* result = mFrameHeap.Alloc(size, DisposeCallbackFunc, nullptr, callback, arg);
    mMutex.Unlock();
    return result;
}

size_t SoundHeap::GetAllocateSize(size_t size, bool align) {
    return align ? nn::util::align_up(size, 0x1000) : size;
}

}  // namespace nn::atk
