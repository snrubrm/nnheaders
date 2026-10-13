#include <nn/atk/detail/Util.h>

namespace nn::atk::detail::Util {

// NON_MATCHING: format-case ordering and return-block placement differ.
size_t GetByteBySample(size_t sampleCount, SampleFormat format) {
    if (static_cast<int>(format) == 0)
        return sampleCount;
    if (static_cast<int>(format) == 1)
        return sampleCount * 2;
    if (static_cast<int>(format) != 2)
        return 0;

    size_t byteCount = (sampleCount / 14) * 8;
    const size_t remainingSamples = sampleCount % 14;
    if (remainingSamples != 0)
        byteCount += 1 + (remainingSamples + 1) / 2;
    return byteCount;
}

bool IsValidMemoryForDsp(const void*, size_t) {
    // The native target implementation accepts every memory range.
    return true;
}

}  // namespace nn::atk::detail::Util
