#include <nn/atk/detail/WaveFile.h>
#include <nn/atk/detail/Util.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

// NON_MATCHING: index extension differs.
const WaveFile::ChannelInfo* WaveFile::InfoBlockBody::GetChannelInfo(int channelIndex) const {
    const ReferenceTable& table = mChannelInfoReferenceTable;
    if (static_cast<u32>(channelIndex) >= table.mCount)
        return nullptr;
    return nn::util::ConstBytePtr(&table, table.mItems[channelIndex].mOffset).Get<ChannelInfo>();
}

const void* WaveFile::ChannelInfo::GetSamplesAddress(const void* dataBlockBodyAddress) const {
    return nn::util::ConstBytePtr(dataBlockBodyAddress, mSamplesOffset).Get();
}

const WaveFile::DspAdpcmInfo* WaveFile::ChannelInfo::GetDspAdpcmInfo() const {
    return nn::util::ConstBytePtr(this, mAdpcmInfoOffset).Get<DspAdpcmInfo>();
}

const WaveFile::InfoBlock* WaveFile::FileHeader::GetInfoBlock() const {
    return static_cast<const InfoBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x7000));
}

const WaveFile::DataBlock* WaveFile::FileHeader::GetDataBlock() const {
    return static_cast<const DataBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x7001));
}

}  // namespace nn::atk::detail
