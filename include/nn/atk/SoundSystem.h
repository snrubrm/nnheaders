#pragma once

#include <nn/atk/AuxBus.h>
#include <nn/atk/OutputMode.h>

namespace nn::atk {

/// The global state of the sound library. TODO: only the effect functions that sead::AudioSystemCafe calls.
class SoundSystem {
public:
    /// 0x710133dc3c (declared only)
    static void Finalize();
    /// 0x710133dfb8 / 0x710133e02c / 0x710133e0b0 (declared only)
    static void ClearEffect(AuxBus bus);
    static void ClearEffect(AuxBus bus, OutputDevice device);
    static bool IsClearEffectFinished(AuxBus bus, OutputDevice device);
};

}  // namespace nn::atk
