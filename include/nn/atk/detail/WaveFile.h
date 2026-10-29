#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the wave resource file (bfwav). Only what the channel getters read.
class WaveFile {
public:
    struct InfoBlock;
    struct DataBlock;

    struct FileHeader {
        // 0x7101364ed8: block type 0x7000
        const InfoBlock* GetInfoBlock() const;
        // 0x7101364f28: block type 0x7001
        const DataBlock* GetDataBlock() const;

    private:
        u8 _0[0x10];
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[2];  // +0x14
    };

    struct DspAdpcmInfo;

    struct ChannelInfo {
        // 0x7101364fa4: the sample reference is relative to the data block body passed in.
        const void* GetSamplesAddress(const void* dataBlockBodyAddress) const;
        // 0x7101364fb0: the ADPCM info offset at +0xc is relative to this.
        const DspAdpcmInfo* GetDspAdpcmInfo() const;

    private:
        u16 mSamplesTypeId;   // +0x0
        u16 _2;
        s32 mSamplesOffset;   // +0x4
        u16 mAdpcmTypeId;     // +0x8
        u16 _a;
        s32 mAdpcmInfoOffset;  // +0xc
    };

    struct InfoBlockBody {
        // 0x7101364f7c: the channel reference table at +0x14 (count, then 8-byte references
        // whose offsets are relative to the table).
        const ChannelInfo* GetChannelInfo(int channelIndex) const;

    private:
        struct Reference {
            u16 mTypeId;
            u16 _2;
            s32 mOffset;
        };
        struct ReferenceTable {
            u32 mCount;
            Reference mItems[1];
        };

        u8 _0[0x14];
        ReferenceTable mChannelInfoReferenceTable;  // +0x14
    };
};

}  // namespace nn::atk::detail
