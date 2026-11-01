/**
 * @file SoundHeap.h
 * @brief Heap that the sound data is loaded to.
 */

#pragma once

#include <nn/types.h>
#include <nn/atk/detail/FrameHeap.h>
#include <nn/os/os_Mutex.h>

namespace nn::atk {

class SoundMemoryAllocatable {
public:
    virtual ~SoundMemoryAllocatable();
    virtual void* Allocate(size_t size) = 0;
    virtual void* Allocate(size_t size, void (*callback)(void*), void* arg) = 0;
    virtual size_t GetAllocateSize(size_t size, bool unk) = 0;
};

/// TODO: only what sead::AudioSoundHeapCafe uses is declared (the class is 0x40 bytes).
class SoundHeap : public SoundMemoryAllocatable {
public:
    SoundHeap();
    ~SoundHeap() override;

    bool Create(void* address, size_t size);
    void Destroy();

    void* Allocate(size_t size) override;
    void* Allocate(size_t size, void (*callback)(void*), void* arg) override;
    size_t GetAllocateSize(size_t size, bool unk) override;

private:
    static void DisposeCallbackFunc(void* address, size_t size, void* arg);

    nn::os::Mutex mMutex;
    detail::FrameHeap mFrameHeap;
};
static_assert(sizeof(SoundHeap) == 0x40);

}  // namespace nn::atk
