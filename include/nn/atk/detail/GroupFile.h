#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the group file (bfgrp): its file header blocks.
class GroupFile {
public:
    struct GroupItemInfo {
        u32 _0;
        Util::Reference mFileReference;
    };
    struct GroupItemInfoEx {
        u32 _0;
        u32 _4;
    };
    struct InfoBlock {
        u32 mSignature;
        u32 mSize;
        Util::ReferenceTable mBody;
    };
    struct FileBlock {
        u32 mSignature;
        u32 mSize;
        u8 mBody[1];
    };
    struct InfoExBlock {
        u32 mSignature;
        u32 mSize;
        Util::ReferenceTable mBody;
    };

    struct FileHeader {
        // 0x710135ecc8 / 0x710135ed1c / 0x710135ed70: block types 0x7800, 0x7801, 0x7802
        const InfoBlock* GetInfoBlock() const;
        const FileBlock* GetFileBlock() const;
        const InfoExBlock* GetInfoExBlock() const;

    private:
    public:
        u32 mSignature;
        u32 _4;
        u32 mVersion;
        u32 _c;
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[3];  // +0x14
    };
};

}  // namespace nn::atk::detail
