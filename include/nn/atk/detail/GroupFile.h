#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the group file (bfgrp): its file header blocks.
class GroupFile {
public:
    struct InfoBlock;
    struct FileBlock;
    struct InfoExBlock;

    struct FileHeader {
        // 0x710135ecc8 / 0x710135ed1c / 0x710135ed70: block types 0x7800, 0x7801, 0x7802
        const InfoBlock* GetInfoBlock() const;
        const FileBlock* GetFileBlock() const;
        const InfoExBlock* GetInfoExBlock() const;

    private:
        u8 _0[0x10];
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[3];  // +0x14
    };
};

}  // namespace nn::atk::detail
