#pragma once

#include <nn/atk/SoundArchive.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the stream sound resource file. The three block descriptors start at +0x14,
// like the archive file header.
class StreamSoundFile {
public:
    struct FileHeader {
        // 0x7101361d8c / 0x7101361da4: the info block is the descriptor with signature 0x4000.
        u32 GetInfoBlockSize() const;
        u32 GetInfoBlockOffset() const;

    private:
        u8 _0[0x14];
        SoundArchiveFile::BlockDescriptor mBlocks[3];
    };
};

}  // namespace nn::atk::detail
