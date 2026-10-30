#include <nn/ui2d/ArcExtractor.h>
#include <nn/font/font_ResFont.h>
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_BytePtr.h>

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

// NON_MATCHING: inlined file-access bounds checks remain; tag branches are not combined.
// 0x710132acec
void ArcExtractor::Unrelocate(const void* archive) {
    ArcExtractor extractor(archive);
    for (s32 i = 0; i < extractor.GetFileCount(); ++i) {
        void* resource = const_cast<void*>(extractor.GetFileFast(nullptr, i));
        // The native dispatch compares the entire eight-byte serialized prefix.
        const u64 signature = *util::ConstBytePtr(resource).Get<u64>();
        if (signature == util::MakeSignature('B', 'N', 'T', 'X') ||
            signature == util::MakeSignature('B', 'N', 'S', 'H')) {
            auto* header = static_cast<util::BinaryFileHeader*>(resource);
            if (header->IsRelocated()) {
                header->GetRelocationTable()->Unrelocate();
                header->SetRelocated(false);
            }
        }
        if (*util::ConstBytePtr(resource).Get<u64>() == util::MakeSignature('F', 'F', 'N', 'T'))
            font::ResFont::Unrelocate(resource);
    }
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

// NON_MATCHING: midpoint-result comparison lowering and instruction scheduling differ.
// 0x710132aed4
s32 ArcExtractor::ConvertPathToEntryId(const char* path) const {
    const u32 multiplier = ReadU32(*util::ConstBytePtr(mFatHeader, 8).Get<u32>());
    u32 hash = 0;
    for (const char* character = path; *character; ++character)
        hash = hash * multiplier + u8(*character);

    s32 left = 0;
    s32 right = mFileCount;
    s32 index = -1;
    for (;;) {
        const s32 middle = (left + right) / 2;
        const u32 entry_hash = ReadU32(mFatEntries[middle].hash);
        if (entry_hash == hash) {
            index = middle;
            break;
        }
        if (entry_hash < hash) {
            if (left == middle)
                break;
            left = middle;
        } else {
            if (right == middle)
                break;
            right = middle;
        }
    }
    if (index == -1)
        return -1;

    const u32 name_info = ReadU32(mFatEntries[index].nameInfo);
    if (name_info == 0)
        return index;
    index = index + 1 - s32(name_info >> 24);
    for (; index < mFileCount; ++index) {
        if (ReadU32(mFatEntries[index].hash) != hash)
            return -1;
        const u32 name_offset = ReadU32(mFatEntries[index].nameInfo) & 0xffffff;
        // Native checks the unscaled offset, then resolves the name in four-byte units.
        if (util::ConstBytePtr(mFntData, name_offset).Get<u8>() > mDataBlock)
            return -1;
        const char* name = util::ConstBytePtr(mFntData, name_offset << 2).Get<char>();
        if (std::strcmp(path, name) == 0)
            break;
    }
    return index;
}

}  // namespace nn::ui2d
