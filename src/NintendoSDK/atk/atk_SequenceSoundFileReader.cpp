#include <nn/atk/detail/SequenceSoundFileReader.h>

namespace nn::atk::detail {

const void* SequenceSoundFileReader::GetSequenceData() const {
    return mSequenceData;
}

bool SequenceSoundFileReader::GetOffsetByLabel(const char* label, u32* offset) const {
    return mLabelBlock->GetOffsetByLabel(label, offset);
}

}  // namespace nn::atk::detail

