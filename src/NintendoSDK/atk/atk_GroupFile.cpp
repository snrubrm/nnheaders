#include <nn/atk/detail/GroupFile.h>

namespace nn::atk::detail {

const GroupFile::InfoBlock* GroupFile::FileHeader::GetInfoBlock() const {
    return static_cast<const InfoBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x7800));
}

const GroupFile::FileBlock* GroupFile::FileHeader::GetFileBlock() const {
    return static_cast<const FileBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x7801));
}

const GroupFile::InfoExBlock* GroupFile::FileHeader::GetInfoExBlock() const {
    return static_cast<const InfoExBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x7802));
}

}  // namespace nn::atk::detail
