#pragma once

#include <nn/atk/detail/fnd/Stream.h>
#include <nn/types.h>

namespace nn::atk::detail::fnd {

// The cache is embedded at +0x28 in FileStreamImpl.
class StreamCache {
public:
    StreamCache();
    virtual ~StreamCache() = default;
    void Initialize(Stream* stream, void* buffer, u64 size);
    void Finalize();
    s32 Read(void* data, u64 size, FndResult* result, FsAccessLog* log, void* unk);
    s32 Write(const void* data, u64 size, FndResult* result);
    s64 Seek(s64 offset, Stream::SeekOrigin origin);

    // Inline-only, name guessed: Read 0x710135cd8c repeats these three tests.
    bool isEnabled() const { return mStream != nullptr && mBuffer != nullptr && mSize != 0; }
    u64 getPosition() const { return mPosition; }

private:
    Stream* mStream;       // +0x08
    u64 mPosition;         // +0x10
    void* mBuffer;         // +0x18
    u64 mSize;             // +0x20
    u64 mCachePosition;    // +0x28
    u64 mCacheSize;        // +0x30
    u8 mCacheState;        // +0x38: Read tests values 1 and 2.
    u8 _39[7];
};
static_assert(sizeof(StreamCache) == 0x40);

}  // namespace nn::atk::detail::fnd
