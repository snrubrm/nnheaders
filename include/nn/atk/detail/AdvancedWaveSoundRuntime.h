/**
 * @file AdvancedWaveSoundRuntime.h
 * @brief Runtime wave sound api.
 */

#pragma once

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

    void Initialize(s32, void**, void const*);
    void Finalize();
    s32 GetActiveCount() const;
    void SetupUserParam(void**, u64);
    void Update();
};
}  // namespace detail
}  // namespace atk
}  // namespace nn
