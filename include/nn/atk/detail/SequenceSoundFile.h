#pragma once

#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial model. Only the label block and its lookup are declared.
class SequenceSoundFile {
public:
    struct DataBlock;
    struct LabelBlock;

    struct FileHeader {
        // 0x7101365024: block type 0x5000
        const DataBlock* GetDataBlock() const;
        // 0x7101365074: block type 0x5001
        const LabelBlock* GetLabelBlock() const;

    private:
        u8 _0[0x10];
        u16 mBlockCount;  // +0x10
        u16 _12;
        Util::ReferenceWithSize mBlocks[2];  // +0x14
    };

    class LabelBlockBody {
    public:
        // 0x71013650c8
        bool GetOffsetByLabel(const char* label, u32* offset) const;
    };
};

}  // namespace nn::atk::detail
