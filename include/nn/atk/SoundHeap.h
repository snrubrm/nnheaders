/**
 * @file SoundHeap.h
 * @brief Heap that the sound data is loaded to.
 */

#pragma once

#include <nn/types.h>

namespace nn::atk {

/// TODO: only what sead::AudioSoundHeapCafe uses is declared (the class is 0x40 bytes).
class SoundHeap {
public:
    SoundHeap();
    virtual ~SoundHeap();

    bool Create(void* address, size_t size);
    void Destroy();

    virtual void* Allocate(size_t size);
    virtual void* Allocate(size_t size, void (*callback)(void*), void* arg);
    virtual size_t GetAllocateSize(size_t size, bool unk);

private:
    u8 _8[0x38];
};
static_assert(sizeof(SoundHeap) == 0x40);

}  // namespace nn::atk
