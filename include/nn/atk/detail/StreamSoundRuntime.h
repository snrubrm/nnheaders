/**
 * @file StreamSoundRuntime.h
 * @brief Stream sound runtime information.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace atk {
class SoundArchive;
namespace detail {
class StreamSoundRuntime {
public:
    StreamSoundRuntime();
    ~StreamSoundRuntime();

    // Static 1350678 reads the archive's player information and scales the
    // caller's size_t cache size; no StreamSoundRuntime receiver is passed.
    static size_t GetRequiredStreamCacheSize(const SoundArchive* archive, size_t cache_size);

    u8 _0[0xB0];
};
}  // namespace detail
}  // namespace atk
}  // namespace nn
