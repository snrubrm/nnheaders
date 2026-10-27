#include <nn/atk/detail/MemoryFileStream.h>
#include <cstring>

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

s64 MemoryFileStream::Write(const void* data, u64 size, fnd::FndResult* result) {
    return 0;
}

s64 MemoryFileStream::Read(void* data, u64 size, fnd::FndResult*) {
    const u64 readable = mSize - mPosition;
    const u64 read_size = readable < size ? readable : size;
    memcpy(data, static_cast<const u8*>(mData) + mPosition, read_size);
    mPosition += read_size;
    return read_size;
}

s64 MemoryFileStream::Seek(s64 offset, fnd::Stream::SeekOrigin origin) {
    switch (origin) {
    case fnd::Stream::SeekOrigin_Begin:
        mPosition = offset;
        return 0;
    case fnd::Stream::SeekOrigin_Current:
        mPosition += offset;
        return 0;
    case fnd::Stream::SeekOrigin_End:
        mPosition = mSize - offset;
        return 0;
    }
    return 0x80000000;
}

s64 MemoryFileStream::Open(const char*, fnd::FileStream::AccessMode) {
    return 0x80000000;
}

fnd::FsAccessLog* MemoryFileStream::SetFsAccessLog(fnd::FsAccessLog* log) {
    return nullptr;
}

}  // namespace nn::atk::detail
