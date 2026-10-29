#include <nn/atk/detail/WaveArchiveFileReader.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

WaveArchiveFileReader::WaveArchiveFileReader()
    : mFile(nullptr), mInfo(nullptr), mFileTable(nullptr), mInitialized(false) {}

WaveArchiveFileReader::WaveArchiveFileReader(const void* file, bool useFileTable)
    : mFile(nullptr), mInfo(nullptr), mFileTable(nullptr), mInitialized(false) {
    Initialize(file, useFileTable);
}

void WaveArchiveFileReader::Initialize(const void* file, bool useFileTable) {
    const auto* header = static_cast<const WaveArchiveFile::FileHeader*>(file);
    if (header && header->mSignature == 0x52415746 && header->mVersion == 0x10000) {
        mFile = header;
        mInfo = &header->GetInfoBlock()->mBody;
        mInitialized = true;
        mFileTable = nullptr;
        if (useFileTable && HasIndividualLoadTable()) {
            mFileTable = nn::util::BytePtr(const_cast<void*>(file), mFile->GetFileBlockOffset())
                             .Advance(4).Get<WaveArchiveFile::FileTableEntry>();
        }
    }
}

bool WaveArchiveFileReader::HasIndividualLoadTable() const {
    if (!mInitialized)
        return false;
    return *nn::util::ConstBytePtr(mFile, mFile->GetFileBlockOffset()).Get<u32>() == 0x54415746;
}

void WaveArchiveFileReader::Finalize() {
    if (mInitialized) {
        mInitialized = false;
        mInfo = nullptr;
        mFileTable = nullptr;
        mFile = nullptr;
    }
}

void WaveArchiveFileReader::InitializeFileTable() {
    for (u32 i = 0; i < (mInitialized ? mInfo->mCount : 0); ++i)
        mFileTable[i].mFile = nullptr;
}

// NON_MATCHING: the prologue and branches are emitted differently.
const void* WaveArchiveFileReader::GetWaveFile(u32 index) const {
    if (!mInitialized || index >= mInfo->mCount)
        return nullptr;
    if (mFileTable)
        return mFileTable[index].mFile;
    return nn::util::ConstBytePtr(mFile->GetFileBlock(), mInfo->mFiles[index].mOffset).Advance(8).Get();
}

u32 WaveArchiveFileReader::GetWaveFileSize(u32 index) const {
    return mInitialized ? mInfo->mFiles[index].mSize : 0;
}

u32 WaveArchiveFileReader::GetWaveFileOffsetFromFileHead(u32 index) const {
    return mInitialized ? mFile->GetFileBlockOffset() + mInfo->mFiles[index].mOffset + 8 : 0;
}

const void* WaveArchiveFileReader::SetWaveFile(u32 index, const void* file) {
    if (!mInitialized || !mFileTable || index >= mInfo->mCount)
        return nullptr;
    const void* old = mFileTable[index].mFile;
    mFileTable[index].mFile = file;
    return old;
}

}  // namespace nn::atk::detail
