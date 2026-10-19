#include <nn/vfx/detail/Utility.h>

#include <cstdarg>

#include <nn/os/os_Mutex.h>
#include <nn/vfx/Heap.h>

#include <cstring>

namespace nn::vfx::detail {

Heap* sStaticHeap;
Heap* sDynamicHeap;
u64 sAllocatedSize;
os::MutexType sDelayFreeMutex;
s32 sDelayFreeCount;
s32 sDelayFreeCapacity;
void** sDelayFreeList;

// The release implementation prepares variable arguments without emitting output.
void OutputError(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    va_end(args);
}

void OutputWarning(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    va_end(args);
}

void OutputLog(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    va_end(args);
}

void SetStaticHeap(Heap* heap) {
    sStaticHeap = heap;
}

Heap* GetStaticHeap() {
    return sStaticHeap;
}

void SetDynamicHeap(Heap* heap) {
    sDynamicHeap = heap;
}

Heap* GetDynamicHeap() {
    return sDynamicHeap;
}

void* AllocFromStaticHeap(size_t size, size_t alignment) {
    const size_t rounded = (size + 0x1f) & ~size_t(0x1f);
    void* ptr = sStaticHeap->Alloc(rounded, alignment);
    sAllocatedSize += rounded;
    return ptr;
}

void* AllocFromDynamicHeap(size_t size, size_t alignment) {
    const size_t rounded = (size + 0xff) & ~size_t(0xff);
    return sDynamicHeap->Alloc(rounded, alignment);
}

void FreeFromStaticHeap(void* ptr) {
    sStaticHeap->Free(ptr);
}

void FreeFromDynamicHeap(void* ptr, bool immediate) {
    if (immediate) {
        sDynamicHeap->Free(ptr);
        return;
    }

    os::LockMutex(&sDelayFreeMutex);
    const s32 index = sDelayFreeCount;
    sDelayFreeList[index] = ptr;
    sDelayFreeCount = index + 1;
    os::UnlockMutex(&sDelayFreeMutex);
}

u64 GetAllocatedSizeFromStaticHeap() {
    return sAllocatedSize;
}

void InitializeDelayFreeList(s32 count) {
    sDelayFreeCapacity = count;
    const size_t bytes = (count * sizeof(void*) + 0x1f) & ~size_t(0x1f);
    void* ptr = sStaticHeap->Alloc(bytes, 0x80);
    sAllocatedSize += bytes;
    sDelayFreeList = static_cast<void**>(ptr);
    memset(sDelayFreeList, 0, sDelayFreeCapacity * sizeof(void*));
    sDelayFreeCount = 0;
}

void FinalizeDelayFreeList() {
    for (s32 i = 0; i < sDelayFreeCount; ++i) {
        sDynamicHeap->Free(sDelayFreeList[i]);
        sDelayFreeList[i] = nullptr;
    }
    sStaticHeap->Free(sDelayFreeList);
    sDelayFreeList = nullptr;
    sDelayFreeCount = 0;
}

void FlushDelayFreeList() {
    for (s32 i = 0; i < sDelayFreeCount; ++i) {
        sDynamicHeap->Free(sDelayFreeList[i]);
        sDelayFreeList[i] = nullptr;
    }
    sDelayFreeCount = 0;
}

}  // namespace nn::vfx::detail
