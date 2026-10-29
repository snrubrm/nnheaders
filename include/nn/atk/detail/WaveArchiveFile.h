#pragma once

#include <nn/atk/SoundArchive.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the wave archive file (bfwar): the binary file header with its two block
// descriptors, info (0x6800) and file (0x6801), at +0x14.
class WaveArchiveFile {
public:
    struct InfoBlock;
    struct FileBlock;

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

        u8 _0[0x14];
        SoundArchiveFile::BlockDescriptor mBlocks[2];  // +0x14
    };
};

}  // namespace nn::atk::detail
