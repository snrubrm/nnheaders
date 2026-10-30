#include <nn/atk/detail/SequenceSoundFile.h>
#include <nn/atk/detail/Util.h>
#include <cstring>

namespace nn::atk::detail {

// NON_MATCHING: the compiler uses a different loop register allocation and stack frame.
bool SequenceSoundFile::LabelBlockBody::GetOffsetByLabel(const char* label, u32* offset) const {
    const size_t length = std::strlen(label);
    const s32 count = mEntries.count;
    for (s32 i = 0; i < count; ++i) {
        const auto* entry = nn::util::ConstBytePtr(mEntries.GetReferedItem(i)).Get<LabelEntry>();
        if (std::strncmp(label, entry->mLabel, length) == 0) {
            *offset = entry->mOffset;
            return true;
        }
    }
    return false;
}

const SequenceSoundFile::DataBlock* SequenceSoundFile::FileHeader::GetDataBlock() const {
    return static_cast<const DataBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x5000));
}

const SequenceSoundFile::LabelBlock* SequenceSoundFile::FileHeader::GetLabelBlock() const {
    return static_cast<const LabelBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x5001));
}

}  // namespace nn::atk::detail
