#pragma once

#include <nn/atk/detail/fnd/Stream.h>
#include <nn/types.h>

namespace nn::atk::detail::fnd {

// Forwards every stream call to the stream at +0x8. The Open/Seek bodies do more than the
// one-call forwarders and are not modelled yet.
class FileStreamProxy : public Stream {
public:
    void Close() override;
    bool IsOpened() const override;
    s32 Read(void* data, u64 size, FndResult* result) override;
    s32 Write(const void* data, u64 size, FndResult* result) override;
    u64 GetCurrentPosition() const override;
    u32 GetSize() const override;
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;
    void Flush() override;
    void EnableCache(void* buffer, u64 size) override;
    void DisableCache() override;
    bool IsCacheEnabled() const override;
    u32 GetIoBufferAlignment() const override;
    bool CanSetFsAccessLog() const override;
    void SetFsAccessLog(FsAccessLog* log) override;

private:
    /* 0x8 */ Stream* mStream;
};

}  // namespace nn::atk::detail::fnd
