#include <nn/atk/detail/WaveArchiveFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

const WaveArchiveFile::InfoBlock* WaveArchiveFile::FileHeader::GetInfoBlock() const {
    return nn::util::ConstBytePtr(this, GetInfoBlockOffset()).Get<InfoBlock>();
}

u32 WaveArchiveFile::FileHeader::GetInfoBlockOffset() const {
    return FindBlock(0x6800)->mOffset;
}

const WaveArchiveFile::FileBlock* WaveArchiveFile::FileHeader::GetFileBlock() const {
    return nn::util::ConstBytePtr(this, GetFileBlockOffset()).Get<FileBlock>();
}

u32 WaveArchiveFile::FileHeader::GetFileBlockOffset() const {
    return FindBlock(0x6801)->mOffset;
}

}  // namespace nn::atk::detail
