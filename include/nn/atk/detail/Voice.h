#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the playback cursor at +0xb0 is modelled; the constructor writes -1 to
// the s32 at +0xac.
class Voice {
public:
    u64 GetPlayPosition() const;

private:
    u8 _0[0xac];
    /* 0xac */ s32 _ac;
    /* 0xb0 */ u64 mPlayPosition;
    u8 _b8[0xd8 - 0xb8];
};

static_assert(sizeof(Voice) == 0xd8);

}  // namespace nn::atk::detail
