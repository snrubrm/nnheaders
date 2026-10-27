#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the bank resource file. Only the table offsets the reference-table getters
// read are modelled.
class BankFile {
public:
    struct InfoBlockBody {
        // 0x7101360110 / 0x710136011c: self-relative table offsets.
        const u32* GetWaveIdTable() const;
        const u32* GetInstrumentReferenceTable() const;

    private:
        u8 _0[4];
        s32 mWaveIdTableOffset;  // +0x4
        u8 _8[4];
        s32 mInstrumentReferenceTableOffset;  // +0xc
    };
};

}  // namespace nn::atk::detail
