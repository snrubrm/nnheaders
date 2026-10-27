#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the advanced wave sound resource file. Only the table offset the
// reference-table getter reads is modelled.
class AdvancedWaveSoundFile {
public:
    struct InfoBlockBody {
        // 0x71013657bc: a self-relative table offset at +0.
        const u32* GetTrackReferenceTable() const;

    private:
        u32 mTrackReferenceTableOffset;  // +0x0
    };
};

}  // namespace nn::atk::detail
