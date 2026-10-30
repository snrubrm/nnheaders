#pragma once

#include <nn/atk/detail/SequenceSoundFile.h>
#include <nn/types.h>

namespace nn::atk::detail {

// The reader holds the file header and the DATA and LABL block bodies.
class SequenceSoundFileReader {
public:
    explicit SequenceSoundFileReader(const void* file);
    const void* GetSequenceData() const;
    // 0x7101361d04
    bool GetOffsetByLabel(const char* label, u32* offset) const;

private:
    const SequenceSoundFile::FileHeader* mFile;
    /* 0x8 */ const void* mSequenceData;
    /* 0x10 */ const SequenceSoundFile::LabelBlockBody* mLabelBlock;
};

}  // namespace nn::atk::detail
