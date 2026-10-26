#pragma once

#include <nn/atk/SampleFormat.h>
#include <nn/atk/detail/Voice.h>
#include <nn/types.h>

namespace nn::atk::detail::driver {

// TODO: incomplete. The object starts with two Voice instances (at +0 and +0xd8) and carries the
// format at +0x24c; only those parts are modelled.
class MultiVoice : public Voice {
public:
    SampleFormat GetFormat() const;
    s64 GetCurrentPlayingSample() const;

private:
    u8 _d8[0x1b0 - 0xd8];
    /* 0x1b0 */ s32 mVoiceCount;
    u8 _1b4[0x24c - 0x1b4];
    /* 0x24c */ SampleFormat mFormat;
};

}  // namespace nn::atk::detail::driver
