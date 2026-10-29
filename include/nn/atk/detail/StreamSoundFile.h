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
        // 0x7101361d0c / 0x7101361d4c: bounded search of the mBlockCount descriptors for the
        // seek (0x4001) and region (0x4003) blocks.
        bool HasSeekBlock() const;
        bool HasRegionBlock() const;
        // 0x7101361dbc
        u32 GetSeekBlockOffset() const;
        // 0x7101361dd8 / 0x7101361df4: data (0x4002) and region (0x4003) block offsets.
        u32 GetDataBlockOffset() const;
        u32 GetRegionBlockOffset() const;

    private:
        // inline-only in the original; name is a guess. The block search repeats in the getters above.
        const SoundArchiveFile::BlockDescriptor* FindBlock(u16 signature) const {
            for (int i = 0; i < mBlockCount; ++i) {
                if (mBlocks[i].mSignature == signature)
                    return &mBlocks[i];
            }
            return nullptr;
        }

        u8 _0[0x10];
        u16 mBlockCount;  // +0x10
        u16 _12;
        SoundArchiveFile::BlockDescriptor mBlocks[3];
    };

    // A typed self-relative reference (type id, offset) as the info block stores it.
    struct Reference {
        u16 mTypeId;    // +0
        u16 _2;
        s32 mOffset;    // +4
    };

    struct DspAdpcmChannelInfo;
    struct StreamSoundInfo;
    struct TrackInfo;
    struct ChannelInfo;

    // A reference table: the item count, then one Reference per item, each relative to the
    // table.
    struct TrackInfoTable {
        // 0x7101361e74: items of type 0x4101.
        const TrackInfo* GetTrackInfo(u32 index) const;

    private:
        u32 mCount;
        Reference mItems[1];
    };

    struct ChannelInfoTable {
        // 0x7101361eb8: items of type 0x4102.
        const ChannelInfo* GetChannelInfo(u32 index) const;

    private:
        u32 mCount;
        Reference mItems[1];
    };

    struct ChannelInfo {
        // 0x7101361efc: reference type 0x300.
        const DspAdpcmChannelInfo* GetDspAdpcmChannelInfo() const;

    private:
        Reference mDetailChannelInfo;  // +0
    };

    struct InfoBlockBody {
        // 0x7101361e10 / 0x7101361e34 / 0x7101361e54: references of type 0x4100 and 0x101
        // (table), each relative to the body.
        const StreamSoundInfo* GetStreamSoundInfo() const;
        const TrackInfoTable* GetTrackInfoTable() const;
        const ChannelInfoTable* GetChannelInfoTable() const;

    private:
        Reference mStreamSoundInfo;   // +0x0
        Reference mTrackInfoTable;    // +0x8
        Reference mChannelInfoTable;  // +0x10
    };
};

}  // namespace nn::atk::detail
