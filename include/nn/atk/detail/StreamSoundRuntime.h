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
    // 0x710135035c / 0x71013503ac (declared only; bodies U).
    size_t GetRequiredStreamBufferSize(const SoundArchive* archive) const;
    static u32 GetRequiredStreamBufferTimes(const SoundArchive* archive);

    u8 _0[0xa8];
    s32 _a8;  // Constructor initializes this multiplier to 5.
    u8 _ac[4];
};
}  // namespace detail
}  // namespace atk
}  // namespace nn
