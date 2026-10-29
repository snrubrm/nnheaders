#include <nn/atk/detail/SequenceSoundFile.h>
#include <nn/atk/detail/Util.h>

namespace nn::atk::detail {

const SequenceSoundFile::DataBlock* SequenceSoundFile::FileHeader::GetDataBlock() const {
    return static_cast<const DataBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x5000));
}

const SequenceSoundFile::LabelBlock* SequenceSoundFile::FileHeader::GetLabelBlock() const {
    return static_cast<const LabelBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x5001));
}

}  // namespace nn::atk::detail
