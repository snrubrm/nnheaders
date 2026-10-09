#include <nn/font/font_GpuBuffer.h>
#include <nn/gfx/gfx_BufferInfo.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <new>

namespace nn::font {

// NON_MATCHING: buffer receiver and pool arguments are loaded in a different order.
bool GpuBuffer::Initialize(gfx::Device* device, const InitializeArg& arg) {
    gfx::BufferInfo info;
    info.SetDefault();
    info.SetGpuAccessFlags(arg.gpuAccessFlags);
    mBufferAlignment = gfx::Buffer::GetBufferAlignment(device, info);
    if (arg.external) {
        mFlags |= 2;
    } else {
        mBufferSize = arg.bufferSize;
        mBufferCount = arg.bufferCount;
        info.SetSize(mBufferSize);
        mBuffers = static_cast<gfx::Buffer*>(
            arg.allocate(sizeof(gfx::Buffer) * mBufferCount, 4, arg.allocatorUserData));
        for (u32 i = 0; i < mBufferCount; ++i) {
            new (&mBuffers[i]) gfx::Buffer;
            mBuffers[i].gfx::detail::BufferImpl<gfx::DefaultApi>::Initialize(
                device, info, arg.memoryPool, arg.memoryPoolOffset + mBufferSize * i, mBufferSize);
        }
        if (arg.shared) {
            mFlags |= 1;
            mSharedOffset = new (arg.allocate(sizeof(std::atomic<u64>), 4, arg.allocatorUserData))
                std::atomic<u64>;
        }
    }
    return true;
}

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
