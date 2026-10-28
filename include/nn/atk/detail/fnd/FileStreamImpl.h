#pragma once

#include <nn/atk/detail/fnd/Stream.h>
#include <nn/fs/fs_types.h>
#include <nn/types.h>

namespace nn::atk::detail::fnd {

// The atk file stream that reads through a stream cache and falls back to the direct file
// handle. The class derives from Stream: its vtable is at 0x710252dab0 and the open flag lives at
// +0x10. Nested DirectStream objects at +0x68 forward to this stream for the cache.
class FileStreamImpl : public Stream {
public:
    // Reads and writes go through the cache buffer while it is enabled.
    class StreamCache {
    public:
        StreamCache();
        // 0x710135cd5c / 0x710135cd74: installs the stream and the aligned buffer range.
        void Initialize(Stream* stream, void* buffer, u64 size);
        void Finalize();
        s32 Read(void* data, u64 size, FndResult* result, FsAccessLog* log, void* unk);
        s32 Write(const void* data, u64 size, FndResult* result);
        s64 Seek(s64 offset, Stream::SeekOrigin origin);

        // Inline-only in the original; name is a guess. StreamCache::Read 0x710135cd8c repeats
        // the same three tests before it uses the cache.
        bool isEnabled() const
        {
            return mStream != nullptr && mBuffer != nullptr && mSize != 0;
        }

        u64 getPosition() const { return mPosition; }

    private:
        u8 _0[8];
        /* 0x08 */ Stream* mStream;
        /* 0x10 */ u64 mPosition;
        /* 0x18 */ void* mBuffer;
        /* 0x20 */ u64 mSize;
        u8 _28[0x40 - 0x28];
    };
    static_assert(sizeof(StreamCache) == 0x40);

    // 0x710135c864: the original body is a four-byte empty function.
    ~FileStreamImpl() override;
    // Nested stream that forwards the read-side queries to this stream at +0x8 of itself.
    class DirectStream : public Stream {
    public:
        void Close() override;
        bool IsOpened() const override;
        s32 Read(void* data, u64 size, FndResult* result) override;
        s32 Write(const void* data, u64 size, FndResult* result) override;
        s64 Seek(s64 offset, Stream::SeekOrigin origin) override;
        u64 GetCurrentPosition() const override;
        u32 GetSize() const override;
        bool CanRead() const override;
        bool CanWrite() const override;
        bool CanSeek() const override;

    private:
        /* 0x8 */ FileStreamImpl* mStream;
    };

    bool IsOpened() const override;
    u32 GetIoBufferAlignment() const override;
    bool CanSetFsAccessLog() const override;
    // 0x710135c400 / 0x710135c40c / 0x710135c418: a stream that is open is readable, writable and
    // seekable.
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;
    // 0x710135c4b4: finalizes the stream cache at +0x28.
    void DisableCache() override;
    // 0x710135c4bc: checks a pointer against the stream's alignment.
    u32 ValidateAlignment(const void* ptr) const;
    // 0x710135c8e0: the binary never writes w0 here.
    void SetFsAccessLog(FsAccessLog* log) override;
    // 0x710135c5b0 / 0x710135c5f4
    void Close() override;
    void Flush() override;
    bool IsCacheEnabled() const override;
    u64 GetCurrentPosition() const override;
    u32 GetSize() const override;
    s32 Write(const void* data, u64 size, FndResult* result) override;

private:
    // 0x710135c684 / 0x710135c710 / 0x710135c790: the uncached file operations.
    s32 ReadDirect(void* data, u64 size, FndResult* result);
    s32 WriteDirect(const void* data, u64 size, FndResult* result);
    s64 SeekDirect(s64 offset, Stream::SeekOrigin origin);

    /* 0x08 */ nn::fs::FileHandle mFileHandle;
    /* 0x10 */ bool mOpened;
    u8 _11[0x20 - 0x11];
    /* 0x20 */ u64 mPosition;
    /* 0x28 */ StreamCache mStreamCache;
    // The embedded DirectStream object at +0x68; not modelled yet because the original Stream
    // interface is narrower than this model's (see the note above the class).
    u8 _68[0x10];
    /* 0x78 */ FsAccessLog* mFsAccessLog;
};
static_assert(sizeof(FileStreamImpl) == 0x80);

}  // namespace nn::atk::detail::fnd
