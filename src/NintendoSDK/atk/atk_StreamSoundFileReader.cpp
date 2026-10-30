#include <nn/atk/detail/StreamSoundFileReader.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

StreamSoundFileReader::StreamSoundFileReader() : mFile(nullptr), mInfo(nullptr) {}

void StreamSoundFileReader::Initialize(const void* file) {
    if (!IsValidFileHeader(file))
        return;
    const auto* header = static_cast<const StreamSoundFile::FileHeader*>(file);
    mFile = header;
    const auto* info = nn::util::ConstBytePtr(header, header->GetInfoBlockOffset())
                           .Get<StreamSoundFile::InfoBlock>();
    if (info->mSignature == 0x4f464e49)
        mInfo = &info->mBody;
}

bool StreamSoundFileReader::IsValidFileHeader(const void* file) {
    const auto* header = static_cast<const StreamSoundFile::FileHeader*>(file);
    return header->mSignature == 0x4d545346 && header->mVersion - 0x10000 <= 0x30000;
}

bool StreamSoundFileReader::IsTrackInfoAvailable() const {
    return mFile->mVersion <= 0x20000;
}

}  // namespace nn::atk::detail
