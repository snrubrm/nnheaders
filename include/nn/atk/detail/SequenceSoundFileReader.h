#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the sequence data pointer at +0x8 is modelled; the file body classes are
// not declared yet.
class SequenceSoundFileReader {
public:
    const void* GetSequenceData() const;

private:
    u8 _0[8];
    /* 0x8 */ const void* mSequenceData;
};

}  // namespace nn::atk::detail
