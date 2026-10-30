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

// NON_MATCHING: record-copy grouping and version selection differ.
bool StreamSoundFileReader::ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* info) const {
    const auto* record = mInfo->GetStreamSoundInfo();
    info->_0 = record->_0;
    info->_1 = record->_1;
    info->_2 = record->_2;
    info->_3 = record->_3;
    info->_4 = record->_4;
    info->_8 = record->_8;
    info->_c = record->_c;
    info->_10 = record->_10;
    info->_14 = record->_14;
    info->_18 = record->_18;
    info->_1c = record->_1c;
    info->_20 = record->_20;
    info->_24 = record->_24;
    info->_28 = record->_28;
    info->_2c = record->_2c;
    info->_30 = record->_30;
    info->_34 = record->_34;
    info->_38 = record->_38;
    info->_3c = record->_3c;
    info->_40 = record->_40;
    info->_44 = mFile->mVersion < 0x40000 ? record->_8 : record->_44;
    info->_48 = mFile->mVersion < 0x40000 ? record->_c : record->_48;
    return true;
}

// NON_MATCHING: channel table loads and the count clamp are optimized differently.
bool StreamSoundFileReader::ReadStreamTrackInfo(TrackInfo* info, int index) const {
    const auto* table = mInfo->GetTrackInfoTable();
    if (!table || index >= static_cast<s32>(table->mCount))
        return false;
    const auto* record = table->GetTrackInfo(index);
    info->_0 = record->_0;
    info->_1 = record->_1;
    info->_2 = record->_2;
    info->_3 = record->_3;
    const auto* channels = nn::util::ConstBytePtr(record, record->mChannelTableOffset).Get<u8>();
    info->mChannelCount = channels[0];
    for (int i = 0; i < info->mChannelCount && i < 2; ++i)
        info->mChannelIndices[i] = channels[4 + i];
    return true;
}

}  // namespace nn::atk::detail
