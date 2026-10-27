#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Partial model. Only the label block and its lookup are declared.
class SequenceSoundFile {
public:
    class LabelBlockBody {
    public:
        // 0x71013650c8
        bool GetOffsetByLabel(const char* label, u32* offset) const;
    };
};

}  // namespace nn::atk::detail
