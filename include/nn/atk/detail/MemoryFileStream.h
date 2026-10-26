#pragma once

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

private:
    u8 _0[8];
    /* 0x8 */ const void* mData;
    /* 0x10 */ u64 mSize;
    /* 0x18 */ u64 mPosition;
};

}  // namespace nn::atk::detail
