#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/atk/detail/StreamSoundFile.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the stream sound prefetch file (bfstp).
class StreamSoundPrefetchFile {
public:
    struct InfoBlock;
    struct PrefetchDataBlock;
    struct PrefetchSample {
        const void* GetSampleAddress() const;
    };

    struct FileHeader {
        // 0x7101364284: block type 0x4000
        const InfoBlock* GetInfoBlock() const;
        // 0x71013642d4: block type 0x4004
        const PrefetchDataBlock* GetPrefetchDataBlock() const;

    public:
        u32 mSignature;
        u32 _4;
        u32 mVersion;
        u32 _c;
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[2];  // +0x14
    };

    struct PrefetchData {
        // 0x7101364328: the sample reference at +0xc, relative to this.
        const PrefetchSample* GetPrefetchSample() const;

        u32 _0;
        u32 _4;
        u8 _8[4];

    private:
        Util::Reference mPrefetchSample;  // +0xc
    };

    struct PrefetchDataBlockBody {
        u32 mCount;
        PrefetchData mData[1];
    };

    struct InfoBlock {
        u32 mSignature;
        u32 mSize;
        StreamSoundFile::InfoBlockBody mBody;
    };
    struct PrefetchDataBlock {
        u32 mSignature;
        u32 mSize;
        PrefetchDataBlockBody mBody;
    };
};

}  // namespace nn::atk::detail
