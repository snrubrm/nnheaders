/**
 * @file WaveSoundRuntime.h
 * @brief Wave sound runtime info.
 */

#pragma once

#include <nn/atk/detail/SoundInstanceManager.h>
#include <nn/types.h>

namespace nn {
namespace atk {
namespace detail {
class WaveSound;

class WaveSoundRuntime : public SoundInstanceManager<WaveSound> {
public:
    WaveSoundRuntime();
    ~WaveSoundRuntime();

    void Initialize(s32, void**, void const*);
    void Finalize();
    s32 GetActiveCount() const;
    s32 GetFreeWaveSoundCount() const;
    void SetupUserParam(void**, u64);
    void Update();

    u8 _30[0x80 - 0x30];
};
}  // namespace detail
}  // namespace atk
}  // namespace nn
