#include <nn/atk/detail/StreamSoundFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

u32 StreamSoundFile::FileHeader::GetInfoBlockSize() const {
    const SoundArchiveFile::BlockDescriptor* block = mBlocks;
    while (block->mSignature != 0x4000)
        ++block;
    return block->mSize;
}

u32 StreamSoundFile::FileHeader::GetInfoBlockOffset() const {
    const SoundArchiveFile::BlockDescriptor* block = mBlocks;
    while (block->mSignature != 0x4000)
        ++block;
    return block->mOffset;
}

bool StreamSoundFile::FileHeader::HasSeekBlock() const {
    return FindBlock(0x4001) != nullptr;
}

bool StreamSoundFile::FileHeader::HasRegionBlock() const {
    return FindBlock(0x4003) != nullptr;
}

u32 StreamSoundFile::FileHeader::GetSeekBlockOffset() const {
    const SoundArchiveFile::BlockDescriptor* block = mBlocks;
    while (block->mSignature != 0x4001)
        ++block;
    return block->mOffset;
}

u32 StreamSoundFile::FileHeader::GetDataBlockOffset() const {
    const SoundArchiveFile::BlockDescriptor* block = mBlocks;
    while (block->mSignature != 0x4002)
        ++block;
    return block->mOffset;
}

u32 StreamSoundFile::FileHeader::GetRegionBlockOffset() const {
    const SoundArchiveFile::BlockDescriptor* block = mBlocks;
    while (block->mSignature != 0x4003)
        ++block;
    return block->mOffset;
}

const StreamSoundFile::TrackInfo* StreamSoundFile::TrackInfoTable::GetTrackInfo(u32 index) const {
    if (index >= mCount)
        return nullptr;
    if (mItems[index].mTypeId != 0x4101)
        return nullptr;
    return nn::util::ConstBytePtr(this, mItems[index].mOffset).Get<TrackInfo>();
}

const StreamSoundFile::ChannelInfo*
StreamSoundFile::ChannelInfoTable::GetChannelInfo(u32 index) const {
    if (index >= mCount)
        return nullptr;
    if (mItems[index].mTypeId != 0x4102)
        return nullptr;
    return nn::util::ConstBytePtr(this, mItems[index].mOffset).Get<ChannelInfo>();
}

const StreamSoundFile::DspAdpcmChannelInfo*
StreamSoundFile::ChannelInfo::GetDspAdpcmChannelInfo() const {
    if (mDetailChannelInfo.mTypeId != 0x300)
        return nullptr;
    return nn::util::ConstBytePtr(this, mDetailChannelInfo.mOffset).Get<DspAdpcmChannelInfo>();
}

const StreamSoundFile::StreamSoundInfo* StreamSoundFile::InfoBlockBody::GetStreamSoundInfo() const {
    if (mStreamSoundInfo.mTypeId != 0x4100)
        return nullptr;
    return nn::util::ConstBytePtr(this, mStreamSoundInfo.mOffset).Get<StreamSoundInfo>();
}

const StreamSoundFile::TrackInfoTable* StreamSoundFile::InfoBlockBody::GetTrackInfoTable() const {
    if (mTrackInfoTable.mTypeId != 0x101)
        return nullptr;
    return nn::util::ConstBytePtr(this, mTrackInfoTable.mOffset).Get<TrackInfoTable>();
}

const StreamSoundFile::ChannelInfoTable*
StreamSoundFile::InfoBlockBody::GetChannelInfoTable() const {
    if (mChannelInfoTable.mTypeId != 0x101)
        return nullptr;
    return nn::util::ConstBytePtr(this, mChannelInfoTable.mOffset).Get<ChannelInfoTable>();
}

}  // namespace nn::atk::detail
