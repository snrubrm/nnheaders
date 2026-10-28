#pragma once

#include <nn/types.h>

namespace nn::atk::detail::fnd {

struct FndResult;
class FsAccessLog;

// Interface of the SDK's plain streams. Only the virtual order needed by the stream classes is
// modelled; the default implementations stay in the platform stream classes.
class Stream {
public:
    // MemoryFileStream::Seek (0x71013543e0) treats value 1 as the end-relative and value 2 as the
    // current-relative origin, so the original enumerators are ordered Begin, End, Current.
    enum SeekOrigin {
        SeekOrigin_Begin,
        SeekOrigin_End,
        SeekOrigin_Current,
    };

    virtual ~Stream() = default;

    virtual void Close() = 0;
    virtual bool IsOpened() const = 0;
    virtual s32 Read(void* data, u64 size, FndResult* result) = 0;
    virtual s32 Write(const void* data, u64 size, FndResult* result) = 0;
    virtual s64 Seek(s64 offset, SeekOrigin origin) = 0;
    virtual u64 GetCurrentPosition() const = 0;
    // 0x710135c638 reads the result with a 32-bit load; MemoryFileStream::GetSize 0x710135445c
    // returns a 64-bit member and is not an override of this interface.
    virtual u32 GetSize() const = 0;
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual bool CanSeek() const = 0;
};

// A stream that can also open a file and cache its reads. FileStreamImpl's vtable continues the
// Stream slots with these eight, and the nested DirectStream (which only implements Stream) has
// the shorter vtable the binary shows at its vtable.
class FileStream : public Stream {
public:
    enum AccessMode {
        AccessMode_Read,
        AccessMode_Write,
        AccessMode_ReadWrite,
    };

    virtual bool Open(const char* path, AccessMode mode) = 0;
    virtual void Flush() = 0;
    virtual void EnableCache(void* buffer, u64 size) = 0;
    virtual void DisableCache() = 0;
    virtual bool IsCacheEnabled() const = 0;
    virtual u32 GetIoBufferAlignment() const = 0;
    virtual bool CanSetFsAccessLog() const = 0;
    virtual void SetFsAccessLog(FsAccessLog* log) = 0;
};

}  // namespace nn::atk::detail::fnd
