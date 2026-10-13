#pragma once

#include <nn/types.h>
#include <nn/atk/SampleFormat.h>

namespace nn::atk::detail {

namespace Util {

size_t GetByteBySample(size_t sampleCount, SampleFormat format);
bool IsValidMemoryForDsp(const void* memory, size_t size);
u32 sub_71013548CC();

/// Lazily constructed global instance (a function-local static in the sound library).
template <typename T>
class Singleton {
public:
    static T& GetInstance();
};

}  // namespace Util

}  // namespace nn::atk::detail
