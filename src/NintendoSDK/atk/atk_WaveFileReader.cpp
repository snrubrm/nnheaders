#include <nn/atk/detail/WaveFileReader.h>

namespace nn::atk::detail {

SampleFormat WaveFileReader::GetSampleFormat(u8 format) {
    switch (format) {
    case 0:
        return static_cast<SampleFormat>(0);
    case 1:
        return static_cast<SampleFormat>(1);
    case 2:
        return static_cast<SampleFormat>(2);
    default:
        return static_cast<SampleFormat>(2);
    }
}

}  // namespace nn::atk::detail
