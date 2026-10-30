#pragma once

#include <nn/atk/AuxBus.h>
#include <nn/atk/OutputMode.h>
#include <nn/audio.h>
#include <nn/types.h>

namespace nn::atk {

/// The global state of the sound library. TODO: only the effect functions that sead::AudioSystemCafe calls.
class SoundSystem {
public:
    /// The settings of the sound library (0x70 bytes).
    struct SoundSystemParam {
        SoundSystemParam();

        u32 _0;
        u8 _4[4];
        size_t _8;
        size_t _10;
        size_t _18;
        u32 _20;
        u8 _24[4];
        size_t _28;
        size_t _30;
        u8 _38[8];
        u32 mSampleRate;
        u32 mEffectCount;
        u32 mVoiceCount;
        u32 _4c;
        u8 _50;
        u8 _51;
        u8 _52;
        u8 _53;
        s32 mRecorderFrameCount;
        u64 _58;
        u8 _60;
        u8 _61;
        u8 _62;
        u8 _63;
        u8 _64;
        u8 _65;
        u8 _66;
        u8 _67;
        u8 _68;
        u8 _69;
        u8 _6a;
        u8 _6b;
        u8 _6c;
        u8 _6d[3];
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
    static void AttachMemoryPool(nn::audio::MemoryPoolType* pool, void* address, size_t size);
    static void DetachMemoryPool(nn::audio::MemoryPoolType* pool);
    static int GetRendererChannelCountMax();
    /// 0x710133e244: the renderer's sample count reported by the driver.
    static int GetRendererSampleCount();
    /// 0x710133dfb8 / 0x710133e02c / 0x710133e0b0 (declared only)
    static void ClearEffect(AuxBus bus);
    static void ClearEffect(AuxBus bus, OutputDevice device);
    static bool IsClearEffectFinished(AuxBus bus, OutputDevice device);
    /// 0x710133e218 (declared only)
    static size_t GetCircularBufferSinkBufferSize();
    /// 0x710133e0e0 (declared only): aal::System::calc passes its frame count when it is positive.
    static void VoiceCommandProcess(u32 frame_count);
};

}  // namespace nn::atk
