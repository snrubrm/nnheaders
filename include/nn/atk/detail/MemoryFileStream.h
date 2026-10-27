#pragma once

#include <nn/atk/detail/fnd/Stream.h>
#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. The fnd::Stream base and the vptr at +0 are not modelled; only the buffer
// fields read by the stream getters are.
class MemoryFileStream {
public:
    bool IsOpened() const;
    u64 GetCurrentPosition() const;
    u64 GetSize() const;
    bool CanRead() const;
    bool CanWrite() const;
    bool CanSeek() const;
    bool IsCacheEnabled() const;
    u32 GetIoBufferAlignment() const;
    bool CanSetFsAccessLog() const;
    void Flush();
    void EnableCache(void*, u64);
    void DisableCache();
    void Close();
    // 0x710135438c: copies at most the remaining bytes and returns the count.
    s64 Read(void* data, u64 size, fnd::FndResult* result);
    // 0x71013543e0: Begin/Current/End; an invalid origin returns the error code.
    s64 Seek(s64 offset, fnd::Stream::SeekOrigin origin);
    // 0x710135447c: a memory stream cannot be opened by path.
    s64 Open(const char* path, fnd::FileStream::AccessMode mode);
    // 0x710135444c / 0x71013544a8: the memory stream cannot write or hold an access log.
    s64 Write(const void* data, u64 size, fnd::FndResult* result);
    // The native body returns the log pointer slot, so it is modelled as returning it.
    fnd::FsAccessLog* SetFsAccessLog(fnd::FsAccessLog* log);

private:
    u8 _0[8];
    /* 0x8 */ const void* mData;
    /* 0x10 */ u64 mSize;
    /* 0x18 */ u64 mPosition;
};

}  // namespace nn::atk::detail
