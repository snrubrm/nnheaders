#include <nn/atk/detail/MemoryFileStream.h>

namespace nn::atk::detail {

bool MemoryFileStream::IsOpened() const {
    return mData != nullptr;
}

u64 MemoryFileStream::GetCurrentPosition() const {
    return mPosition;
}

u64 MemoryFileStream::GetSize() const {
    return mSize;
}

bool MemoryFileStream::CanRead() const {
    return true;
}

bool MemoryFileStream::CanWrite() const {
    return false;
}

bool MemoryFileStream::CanSeek() const {
    return true;
}

bool MemoryFileStream::IsCacheEnabled() const {
    return false;
}

u32 MemoryFileStream::GetIoBufferAlignment() const {
    return 1;
}

bool MemoryFileStream::CanSetFsAccessLog() const {
    return false;
}

void MemoryFileStream::Flush() {}

void MemoryFileStream::EnableCache(void*, u64) {}

void MemoryFileStream::DisableCache() {}

void MemoryFileStream::Close() {
    mSize = 0;
    mPosition = 0;
    mData = nullptr;
}

}  // namespace nn::atk::detail
