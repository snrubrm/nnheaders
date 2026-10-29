#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the wave sound resource file. Only the table offset the reference-table
// getter reads is modelled.
class WaveSoundFile {
public:
    struct InfoBlock;

    struct FileHeader {
        // 0x7101364534: block type 0x6800
        const InfoBlock* GetInfoBlock() const;

    private:
        u8 _0[0x10];
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[1];  // +0x14
    };

    struct AdshrCurve;

    struct WaveSoundInfo {
        // 0x7101364638..0x7101364838: optional values of the option flags at +0 (see
        // Util::ReadOptionParameter).
        u8 GetPan() const;
        u8 GetSurroundPan() const;
        f32 GetPitch() const;
        u8 GetLpfFreq() const;
        u8 GetBiquadType() const;
        u16 GetBiquadValue() const;

    private:
        u32 mOptionFlags;  // +0x0, followed by the present optional values
    };

    struct NoteInfo {
        // 0x7101364864..0x71013648f0: the optional values follow the option flags at +4 (see Util::ReadOptionParameter).
        u32 GetOriginalKey() const;
        u32 GetVolume() const;
        u8 GetPan() const;
        u8 GetSurroundPan() const;
        f32 GetPitch() const;

    private:
        u32 mWaveIdTableIndex;  // +0x0
        u32 mOptionFlags;       // +0x4, followed by the present optional values
    };


    struct WaveSoundData {
        // 0x71013645e0: the info reference at +0 (offset at +4) is relative to this.
        const WaveSoundInfo* GetWaveSoundInfo() const;
        // 0x71013645ec: the note table reference at +0x10 (offset at +0x14) points at a count
        // and 8-byte {type, offset} references of type 0x4902, relative to the table.
        const NoteInfo* GetNoteInfo(u32 index) const;

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

        Reference mWaveSoundInfo;   // +0x0
        Reference mTrackInfoTable;  // +0x8
        Reference mNoteInfoTable;   // +0x10
    };

    struct InfoBlockBody {
        // 0x71013645d4: a self-relative table offset.
        const u32* GetWaveIdTable() const;
        // 0x7101364588: the wave sound data reference table (items of type 0x4900).
        const WaveSoundData* GetWaveSoundData(u32 index) const;

    private:
        u8 _0[4];
        s32 mWaveIdTableOffset;  // +0x4
        u8 _8[4];
        s32 mWaveSoundDataReferenceTableOffset;  // +0xc
    };
};

}  // namespace nn::atk::detail
