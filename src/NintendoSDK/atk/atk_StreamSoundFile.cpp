#include <nn/atk/detail/StreamSoundFile.h>

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

}  // namespace nn::atk::detail
