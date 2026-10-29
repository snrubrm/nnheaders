#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the bank resource file. Only the table offsets the reference-table getters
// read are modelled.
class BankFile {
public:
    struct InfoBlock;
    struct Instrument;

    struct FileHeader {
        // 0x71013600bc: block type 0x5800
        const InfoBlock* GetInfoBlock() const;

    public:
        u32 mSignature;
        u32 _4;
        u32 mVersion;
        u32 _c;
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[1];  // +0x14
    };

    struct VelocityRegion {
        // 0x7101360350..0x7101360460: the optional values follow the option flags at +4 (see Util::ReadOptionParameter).
        u32 GetOriginalKey() const;
        u32 GetVolume() const;
        u8 GetPan() const;
        f32 GetPitch() const;
        bool IsIgnoreNoteOff() const;
        u8 GetKeyGroup() const;
        u16 GetInterpolationType() const;

    private:
        u32 mWaveIdTableIndex;  // +0x0
        u32 mOptionFlags;       // +0x4, followed by the present optional values
    };

    struct InfoBlockBody {
        // 0x7101360110 / 0x710136011c: self-relative table offsets.
        const u32* GetWaveIdTable() const;
        const u32* GetInstrumentReferenceTable() const;
        // 0x7101360128: the item type (0x5900) is checked before the index bound.
        const Instrument* GetInstrument(int index) const;

    private:
        u8 _0[4];
        s32 mWaveIdTableOffset;  // +0x4
        u8 _8[4];
        s32 mInstrumentReferenceTableOffset;  // +0xc
    };

    struct InfoBlock {
        u32 mSignature;
        u32 mSize;
        InfoBlockBody mBody;
    };
};

}  // namespace nn::atk::detail
