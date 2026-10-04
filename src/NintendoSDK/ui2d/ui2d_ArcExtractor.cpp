#include <nn/ui2d/ArcExtractor.h>

#include <cstring>

namespace nn::ui2d {

// 0x710132aac8
ArcExtractor::ArcExtractor(const void* archive)
    : mArchive(nullptr), mFatHeader(nullptr), mFntData(nullptr), mFatEntries(nullptr), mFileCount(0),
      mDataBlock(nullptr), mIsNativeEndian(1) {
    PrepareArchive(archive);
}

// 0x710132ace8
ArcExtractor::~ArcExtractor() {}

// NON_MATCHING: same checks and stores; ours widens the (16 bit) node count times 16 before the add (one extra
// instruction) and orders the byte order comparisons differently
// 0x710132aae4
bool ArcExtractor::PrepareArchive(const void* archive) {
    if (!archive)
        return false;
    mArchive = archive;

    const char* bytes = static_cast<const char*>(archive);
    if (std::strncmp(bytes, "SARC", 4) != 0)
        return false;

    // The byte order mark (ff fe in a little endian archive)
    const u16 byte_order = *reinterpret_cast<const u16*>(bytes + 6);
    const u8 low = byte_order & 0xff;
    const u8 high = byte_order >> 8;
    if (low == 0xff && high == 0xfe)
        mIsNativeEndian = 1;
    else
        mIsNativeEndian = (high != 0xff) | (low != 0xfe);

    if (ReadU16(*reinterpret_cast<const u16*>(bytes + 0x10)) != 0x100)
        return false;
    if (ReadU16(*reinterpret_cast<const u16*>(bytes + 4)) != 0x14)
        return false;

    mFatHeader = bytes + 0x14;
    if (std::strncmp(bytes + 0x14, "SFAT", 4) != 0)
        return false;
    if (ReadU16(*reinterpret_cast<const u16*>(bytes + 0x18)) != 0xc)
        return false;
    const u16 node_count = ReadU16(*reinterpret_cast<const u16*>(bytes + 0x1a));
    if ((node_count >> 14) != 0)
        return false;
    mFileCount = static_cast<s16>(node_count);

    mFatEntries = reinterpret_cast<const FatEntry*>(
        bytes + static_cast<s16>(ReadU16(*reinterpret_cast<const u16*>(bytes + 4))) +
        static_cast<s16>(ReadU16(*reinterpret_cast<const u16*>(bytes + 0x18))));
    const char* fnt = bytes + static_cast<s16>(ReadU16(*reinterpret_cast<const u16*>(bytes + 4))) +
                    static_cast<s16>(ReadU16(*reinterpret_cast<const u16*>(bytes + 0x18))) +
                    static_cast<s16>(ReadU16(*reinterpret_cast<const u16*>(bytes + 0x1a))) * 0x10;
    if (std::strncmp(fnt, "SFNT", 4) != 0)
        return false;
    if (ReadU16(*reinterpret_cast<const u16*>(fnt + 4)) != 8)
        return false;
    mFntData = fnt + 8;

    const s64 data_offset = static_cast<s32>(ReadU32(*reinterpret_cast<const u32*>(bytes + 0xc)));
    if (data_offset < fnt + 8 - bytes)
        return false;
    mDataBlock = reinterpret_cast<const u8*>(bytes + data_offset);
    return true;
}

// 0x710132ae50
s32 ArcExtractor::GetFileCount() const {
    return mFileCount < 0 ? 0 : mFileCount;
}

// NON_MATCHING: same code, register allocation only
// 0x710132ae5c
const void* ArcExtractor::GetFileFast(ArcFileInfo* info, s32 entry_id) {
    if (entry_id < 0 || entry_id >= mFileCount)
        return nullptr;

    const u32 start = ReadU32(mFatEntries[entry_id].dataStart);
    if (info) {
        const u32 end = ReadU32(mFatEntries[entry_id].dataEnd);
        if (end < start)
            return nullptr;
        info->offset = start;
        info->size = end - start;
    }
    return mDataBlock + start;
}

}  // namespace nn::ui2d
