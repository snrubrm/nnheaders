#include <nn/atk/detail/StreamSoundPrefetchFileReader.h>

namespace nn::atk::detail {

StreamSoundPrefetchFileReader::StreamSoundPrefetchFileReader()
    : mFile(nullptr), mInfo(nullptr), mPrefetchData(nullptr) {}

void StreamSoundPrefetchFileReader::Initialize(const void* file) {
    const auto* header = static_cast<const StreamSoundPrefetchFile::FileHeader*>(file);
    if (header->mSignature != 0x50545346 || header->mVersion - 0x10000 > 0x10100)
        return;
    mFile = header;
    const auto* info = header->GetInfoBlock();
    if (info->mSignature != 0x4f464e49)
        return;
    const auto* data = mFile->GetPrefetchDataBlock();
    if (data->mSignature != 0x54414450)
        return;
    mInfo = &info->mBody;
    mPrefetchData = data->mBody;
}

bool StreamSoundPrefetchFileReader::ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* info) const {
    *info = *mInfo->GetStreamSoundInfo();
    return true;
}

}  // namespace nn::atk::detail
