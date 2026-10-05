/**
 * @file ArcExtractor.h
 * @brief Reader of the layout archive (ARC) format.
 */

#pragma once

#include <nn/types.h>

namespace nn::ui2d {

// Offset and size of a file inside the archive's data block.
struct ArcFileInfo {
    u32 offset;
    u32 size;
};

// Layout evidence: ArcExtractor::ArcExtractor / PrepareArchive (0x710132aac8 / 0x710132aae4). The archive's values are
// stored in either byte order; `mIsNativeEndian` (from the byte order mark) tells whether they have to be swapped.
class ArcExtractor {
public:
    explicit ArcExtractor(const void* archive);
    ~ArcExtractor();

    // Checks the headers and sets up the members; false for an invalid archive.
    bool PrepareArchive(const void* archive);
    // Releases the relocated state of every font in the archive (walks the files, ResFont::Unrelocate).
    static void Unrelocate(const void* archive);

    s32 GetFileCount() const;
    const void* GetFileFast(ArcFileInfo* info, s32 entry_id);
    s32 ConvertPathToEntryId(const char* path) const;

private:
    // An entry of the file allocation table (0x10 bytes).
    struct FatEntry {
        u32 hash;
        u32 nameInfo;  // name offset (24 bits) and collision count (8 bits)
        u32 dataStart;
        u32 dataEnd;
    };

    u16 ReadU16(u16 value) const {
        if (mIsNativeEndian == 0)
            return __builtin_bswap16(value);
        return value;
    }

    u32 ReadU32(u32 value) const {
        if (mIsNativeEndian == 0)
            return __builtin_bswap32(value);
        return value;
    }

public:
    // Read directly by eui::MultiArcResourceAccessor::isArchiveAttached.
    // Public access is inferred; the archive pointer and its producer are verified.
    /* 0x00 */ const void* mArchive;

private:
    /* 0x08 */ const void* mFatHeader;
    /* 0x10 */ const void* mFntData;
    /* 0x18 */ const FatEntry* mFatEntries;
    /* 0x20 */ s32 mFileCount;
    /* 0x28 */ const u8* mDataBlock;
    /* 0x30 */ u32 mIsNativeEndian;
};

}  // namespace nn::ui2d
