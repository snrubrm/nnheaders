#pragma once

#include <nn/atk/SampleFormat.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Static interface only; the reader's instance representation remains opaque.
class WaveFileReader {
public:
    static SampleFormat GetSampleFormat(u8 format);
};

}  // namespace nn::atk::detail
