#pragma once

#include <nn/atk/detail/SequenceSoundFile.h>
#include <nn/types.h>

namespace nn::atk::detail {

// TODO: incomplete. Only the sequence data pointer at +0x8 is modelled; the file body classes are
// not declared yet.
class SequenceSoundFileReader {
public:
    const void* GetSequenceData() const;
    // 0x7101361d04
    bool GetOffsetByLabel(const char* label, u32* offset) const;

private:
    u8 _0[8];
    /* 0x8 */ const void* mSequenceData;
    /* 0x10 */ const SequenceSoundFile::LabelBlockBody* mLabelBlock;
};

}  // namespace nn::atk::detail
