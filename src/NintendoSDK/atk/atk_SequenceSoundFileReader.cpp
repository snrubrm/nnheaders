#include <nn/atk/detail/SequenceSoundFileReader.h>

namespace nn::atk::detail {

SequenceSoundFileReader::SequenceSoundFileReader(const void* file)
    : mFile(nullptr), mSequenceData(nullptr), mLabelBlock(nullptr) {
    const auto* header = static_cast<const SequenceSoundFile::FileHeader*>(file);
    if (header->mSignature != 0x51455346 || header->mVersion - 0x10000 > 0x10000)
        return;
    mFile = header;
    const auto* data = header->GetDataBlock();
    if (data->mSignature != 0x41544144)
        return;
    const auto* label = mFile->GetLabelBlock();
    if (label->mSignature != 0x4c42414c)
        return;
    mSequenceData = data->mBody;
    mLabelBlock = &label->mBody;
}

const void* SequenceSoundFileReader::GetSequenceData() const {
    return mSequenceData;
}

bool SequenceSoundFileReader::GetOffsetByLabel(const char* label, u32* offset) const {
    return mLabelBlock->GetOffsetByLabel(label, offset);
}

}  // namespace nn::atk::detail

