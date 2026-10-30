#pragma once

#include <nn/atk/SoundHeap.h>
#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {
class SoundPlayer;
}

namespace nn::atk::detail {

// Partial native model: the allocator base and observed ownership/links are represented.
class PlayerHeap : public SoundMemoryAllocatable {
public:
    PlayerHeap();
    ~PlayerHeap() override;

    void* Allocate(size_t size) override;
    void* Allocate(size_t size, void (*callback)(void*), void* arg) override;
    size_t GetAllocateSize(size_t size, bool withCallback) override;

private:
    SoundPlayer* mPlayer;  // +0x8
    void* mBegin;          // +0x10
    void* mEnd;            // +0x18
    void* mCursor;         // +0x20
    u8 _28;
    u8 _29[7];
    nn::util::IntrusiveListNode mPlayerLink;                     // +0x30
    nn::util::detail::IntrusiveListImplementation mCallbacks;   // +0x40
};

}  // namespace nn::atk::detail
