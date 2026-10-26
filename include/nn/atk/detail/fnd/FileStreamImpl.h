#pragma once

#include <nn/types.h>

namespace nn::atk::detail::fnd {

// TODO: incomplete. The FileStream base and its stream cache are not modelled; the open flag
// lives at +0x10.
class FileStreamImpl {
public:
    bool IsOpened() const;
    u32 GetIoBufferAlignment() const;
    bool CanSetFsAccessLog() const;

private:
    u8 _0[0x10];
    /* 0x10 */ bool mOpened;
};

}  // namespace nn::atk::detail::fnd
