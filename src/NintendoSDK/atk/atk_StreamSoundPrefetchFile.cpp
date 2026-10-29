#include <nn/atk/detail/StreamSoundPrefetchFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

const StreamSoundPrefetchFile::InfoBlock*
StreamSoundPrefetchFile::FileHeader::GetInfoBlock() const {
    return static_cast<const InfoBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x4000));
}

const StreamSoundPrefetchFile::PrefetchDataBlock*
StreamSoundPrefetchFile::FileHeader::GetPrefetchDataBlock() const {
    return static_cast<const PrefetchDataBlock*>(
        Util::FindBlock(this, mBlockCount, mBlocks, 0x4004));
}

const StreamSoundPrefetchFile::PrefetchSample*
StreamSoundPrefetchFile::PrefetchData::GetPrefetchSample() const {
    return nn::util::ConstBytePtr(this, mPrefetchSample.offset).Get<PrefetchSample>();
}

}  // namespace nn::atk::detail
