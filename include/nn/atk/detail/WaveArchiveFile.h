#pragma once

#include <nn/atk/SoundArchive.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the wave archive file (bfwar): the binary file header with its two block
// descriptors, info (0x6800) and file (0x6801), at +0x14.
class WaveArchiveFile {
public:
    struct WaveFileReference {
        u16 mType;
        u16 _2;
        u32 mOffset;
        u32 mSize;
    };
    struct InfoBlockBody {
        u32 mCount;
        WaveFileReference mFiles[1];
    };
    struct InfoBlock {
        u32 mSignature;
        u32 mSize;
        InfoBlockBody mBody;
    };
    struct FileBlock;
    struct FileTableEntry {
        const void* mFile;
        u64 _8;
    };

    struct FileHeader {
        // 0x7101343940 / 0x7101343970 / 0x7101343998 / 0x71013439c8
        const InfoBlock* GetInfoBlock() const;
        u32 GetInfoBlockOffset() const;
        const FileBlock* GetFileBlock() const;
        u32 GetFileBlockOffset() const;

    private:
        // inline-only in the original; name is a guess. The block search repeats in the getters above.
        const SoundArchiveFile::BlockDescriptor* FindBlock(u16 signature) const {
            for (int i = 0; i < 2; ++i) {
                if (mBlocks[i].mSignature == signature)
                    return &mBlocks[i];
            }
            return nullptr;
        }

    public:
        u32 mSignature;
        u32 _4;
        u32 mVersion;
        u8 _c[8];
        SoundArchiveFile::BlockDescriptor mBlocks[2];  // +0x14
    };
};

}  // namespace nn::atk::detail
