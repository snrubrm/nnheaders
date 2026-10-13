#include <nn/atk/detail/WaveFileReader.h>

namespace nn::atk::detail {

// NON_MATCHING: the compiler selects the result instead of branching to returns.
SampleFormat WaveFileReader::GetSampleFormat(u8 format) {
    if (format <= 2)
        return static_cast<SampleFormat>(format);
    return static_cast<SampleFormat>(2);
}

}  // namespace nn::atk::detail
