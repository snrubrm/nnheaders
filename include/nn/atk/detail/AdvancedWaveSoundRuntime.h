/**
 * @file AdvancedWaveSoundRuntime.h
 * @brief Runtime wave sound api.
 */

#pragma once

#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/SoundInstanceManager.h>
#include <nn/types.h>

namespace nn {
namespace atk {
namespace detail {
class AdvancedWaveSound;

class AdvancedWaveSoundRuntime : public SoundInstanceManager<AdvancedWaveSound> {
public:
    AdvancedWaveSoundRuntime();
    ~AdvancedWaveSoundRuntime();

    // Size return inferred from allocation-size callers.
    static size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& info,
                                        size_t alignment);
    void Initialize(s32, void**, void const*);
    void Finalize();
    s32 GetActiveCount() const;
    void SetupUserParam(void**, u64);
    void Update();
};
}  // namespace detail
}  // namespace atk
}  // namespace nn
