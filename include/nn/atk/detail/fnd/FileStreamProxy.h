#pragma once

#include <nn/types.h>

namespace nn::atk::detail::fnd {

struct FndResult;
class FsAccessLog;

class FileStream {
public:
    enum AccessMode {
        AccessMode_Read,
        AccessMode_Write,
        AccessMode_ReadWrite,
    };
};

// Interface of the SDK's file streams. Only the virtual order needed by FileStreamProxy is
// modelled; the default implementations stay in the platform stream classes.
class Stream {
public:
    enum SeekOrigin {
        SeekOrigin_Begin,
        SeekOrigin_Current,
        SeekOrigin_End,
    };

    virtual ~Stream() = default;

    virtual void Close() = 0;
    virtual bool IsOpened() const = 0;
    virtual s32 Read(void* data, u64 size, FndResult* result) = 0;
    virtual s32 Write(const void* data, u64 size, FndResult* result) = 0;
    virtual s64 Seek(s64 offset, SeekOrigin origin) = 0;
    virtual u64 GetCurrentPosition() const = 0;
    virtual u64 GetSize() const = 0;
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual bool CanSeek() const = 0;
    virtual bool Open(const char* path, FileStream::AccessMode mode) = 0;
    virtual void Flush() = 0;
    virtual void EnableCache(void* buffer, u64 size) = 0;
    virtual void DisableCache() = 0;
    virtual bool IsCacheEnabled() const = 0;
    virtual u32 GetIoBufferAlignment() const = 0;
    virtual bool CanSetFsAccessLog() const = 0;
    virtual bool SetFsAccessLog(FsAccessLog* log) = 0;
};

// Forwards every stream call to the stream at +0x8. The Open/Seek bodies do more than the
// one-call forwarders and are not modelled yet.
class FileStreamProxy : public Stream {
public:
    void Close() override;
    bool IsOpened() const override;
    s32 Read(void* data, u64 size, FndResult* result) override;
    s32 Write(const void* data, u64 size, FndResult* result) override;
    u64 GetCurrentPosition() const override;
    u64 GetSize() const override;
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;
    void Flush() override;
    void EnableCache(void* buffer, u64 size) override;
    void DisableCache() override;
    bool IsCacheEnabled() const override;
    u32 GetIoBufferAlignment() const override;
    bool CanSetFsAccessLog() const override;
    bool SetFsAccessLog(FsAccessLog* log) override;

private:
    /* 0x8 */ Stream* mStream;
};

}  // namespace nn::atk::detail::fnd
