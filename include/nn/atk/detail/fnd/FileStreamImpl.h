#pragma once

#include <nn/atk/detail/fnd/Stream.h>
#include <nn/types.h>

namespace nn::atk::detail::fnd {

// TODO: incomplete. Only the members the stream queries read are modelled; the stream cache is
// not. The class derives from Stream: its vtable is at 0x710252dab0 and the open flag lives at
// +0x10.
class FileStreamImpl : public Stream {
public:
    // 0x710135c864: the original body is a four-byte empty function.
    ~FileStreamImpl() override;
    // Nested stream that forwards the read-side queries to the stream at +0x8.
    class DirectStream : public Stream {
    public:
        void Close() override;
        bool IsOpened() const override;
        u64 GetSize() const override;
        bool CanRead() const override;
        bool CanWrite() const override;
        bool CanSeek() const override;

    private:
        /* 0x8 */ Stream* mStream;
    };

    bool IsOpened() const override;
    u32 GetIoBufferAlignment() const override;
    bool CanSetFsAccessLog() const override;
    // 0x710135c400 / 0x710135c40c / 0x710135c418: a stream that is open is readable, writable and
    // seekable.
    bool CanRead() const override;
    bool CanWrite() const override;
    bool CanSeek() const override;
    // 0x710135c4bc: checks a pointer against the stream's alignment.
    u32 ValidateAlignment(const void* ptr) const;
    // 0x710135c8e0: the binary never writes w0 here.
    void SetFsAccessLog(FsAccessLog* log) override;

private:
    u8 _8[0x10 - 0x8];
    /* 0x10 */ bool mOpened;
    u8 _11[0x78 - 0x11];
    /* 0x78 */ FsAccessLog* mFsAccessLog;
};

}  // namespace nn::atk::detail::fnd
