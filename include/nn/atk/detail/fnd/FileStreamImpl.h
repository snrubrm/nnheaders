#pragma once

#include <nn/atk/detail/fnd/Stream.h>
#include <nn/types.h>

namespace nn::atk::detail::fnd {

// TODO: incomplete. The FileStream base and its stream cache are not modelled; the open flag
// lives at +0x10.
class FileStreamImpl {
public:
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

    bool IsOpened() const;
    u32 GetIoBufferAlignment() const;
    bool CanSetFsAccessLog() const;
    // 0x710135c8e0. The stream is a partial standalone model here, so the native void return is used.
    void SetFsAccessLog(FsAccessLog* log);

private:
    u8 _0[0x10];
    /* 0x10 */ bool mOpened;
    u8 _11[0x78 - 0x11];
    /* 0x78 */ FsAccessLog* mFsAccessLog;
};

}  // namespace nn::atk::detail::fnd
