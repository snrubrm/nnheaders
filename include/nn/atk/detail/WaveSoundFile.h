#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the wave sound resource file. Only the table offset the reference-table
// getter reads is modelled.
class WaveSoundFile {
public:
    struct InfoBlockBody {
        // 0x71013645d4: a self-relative table offset.
        const u32* GetWaveIdTable() const;

    private:
        u8 _0[4];
        s32 mWaveIdTableOffset;  // +0x4
    };
};

}  // namespace nn::atk::detail
