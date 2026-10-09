#include <nn/font/font_GpuBuffer.h>

namespace nn::font {

void GpuBuffer::Map(s32 index) {
    if (mMappedIndex >= 0)
        return;
    mMappedIndex = index;
    if (mBuffers)
        mMappedPtr = mBuffers[index].Map();
    if (mFlags & 1)
        mSharedOffset->store(0, std::memory_order_release);
    else
        mOffset = 0;
}

void GpuBuffer::Unmap() {
    if (mMappedIndex < 0)
        return;
    // Native shared allocation uses acquire/release operations, including this discarded acquire read.
    if (mFlags & 1)
        mSharedOffset->load(std::memory_order_acquire);
    if (mBuffers)
        mBuffers[mMappedIndex].Unmap();
    mMappedIndex = -1;
    mMappedPtr = nullptr;
    if (mFlags & 1)
        mSharedOffset->store(0, std::memory_order_release);
    else
        mOffset = 0;
}

}  // namespace nn::font
