#pragma once

#include <nn/atk/AuxBus.h>
#include <nn/atk/OutputMode.h>
#include <nn/types.h>

namespace nn::atk {

/// The global state of the sound library. TODO: only the effect functions that sead::AudioSystemCafe calls.
class SoundSystem {
public:
    /// The settings of the sound library (0x70 bytes; the fields are not known).
    struct SoundSystemParam {
        /// 0x710133d208 (declared only)
        SoundSystemParam();

        u8 _0[0x70];
    };
    static_assert(sizeof(SoundSystemParam) == 0x70, "nn::atk::SoundSystem::SoundSystemParam size mismatch");

    /// 0x710133d278 (declared only): the size of the memory that Initialize needs.
    static size_t GetRequiredMemSize(const SoundSystemParam& param);
    /// 0x710133da24 (declared only)
    static bool Initialize(const SoundSystemParam& param, size_t memory, size_t memory_size);
    /// 0x710133dc3c (declared only)
    static void Finalize();
    /// 0x710133dd80 (declared only)
    static bool IsInitialized();
    /// 0x710133dfb8 / 0x710133e02c / 0x710133e0b0 (declared only)
    static void ClearEffect(AuxBus bus);
    static void ClearEffect(AuxBus bus, OutputDevice device);
    static bool IsClearEffectFinished(AuxBus bus, OutputDevice device);
};

}  // namespace nn::atk
