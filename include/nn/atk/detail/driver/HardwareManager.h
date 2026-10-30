#pragma once

#include <nn/atk/OutputMode.h>
#include <nn/atk/AuxBus.h>
#include <nn/audio.h>
#include <nn/atk/detail/Util.h>
#include <nn/os.h>
#include <nn/os/os_Mutex.h>
#include <nn/types.h>

namespace nn::atk::detail {

/// Declared only: the voice allocator's required-size query 0x7101356d6c is still undecompiled.
class LowLevelVoiceAllocator {
public:
    static int GetRequiredMemSize(int max_voices);
};

}  // namespace nn::atk::detail

namespace nn::atk {
class EffectAux;
}

namespace nn::atk::detail::driver {

class HardwareManager {
public:
    struct HardwareManagerParameter;
    /// The master volume, which is faded linearly from the previous to the target volume over the frames that
    /// were given to SetMasterVolume (see sead::AudioResetterCafe::calc, 0x7100bb9858).
    f32 GetMasterVolume() const {
        if (mMasterVolumeCurrentFrame >= mMasterVolumeFrames)
            return mMasterVolumeTarget;
        return mMasterVolumeStart + mMasterVolumeCurrentFrame *
                                        (mMasterVolumeTarget - mMasterVolumeStart) /
                                        mMasterVolumeFrames;
    }

    // Inline-only, names guessed: full initialization 13449C4 and independent
    // pool consumer 1344714 establish the renderer configuration at +0x18.
    // Constructor 1343DB4 and SoundSystem query 133DD80 establish the flag at +0.
    bool IsInitialized() const { return mIsInitialized; }
    nn::audio::AudioRendererConfig* GetAudioRendererConfig() { return &mAudioRendererConfig; }
    // Inline-only in the original; name is a guess. Renderer operations 1344688 and 1344714
    // both lock the mutex at +0x760 around nn::audio calls.
    nn::os::MutexType* GetAudioRendererMutex() { return &mAudioRendererMutex; }

    void SetMasterVolume(f32 volume, s32 frames);
    // Native 1344670 returns the renderer channel count (2 or 6). The private
    // configuration flag it reads remains outside this partial representation.
    int GetChannelCountMax() const;
    /// 0x7101344564: forwards the voice count to LowLevelVoiceAllocator::GetRequiredMemSize.
    int GetRequiredMemSizeForMemoryPool(int max_voices) const;
    size_t GetRequiredEffectAuxBufferSize(const EffectAux* effect) const;
    size_t GetRequiredPerformanceFramesBufferSize(const HardwareManagerParameter& parameter) const;
    bool IsFinishedClearEffect(AuxBus bus, OutputDevice device, int index) const;
    /// 0x71013450b8 (declared only)
    void SetOutputMode(OutputMode mode, OutputDevice device);
    /// 0x7101347174: the native body is empty on this platform.
    void FlushDataCache(void* address, size_t size);
    /// Inline-only, name guessed: the renderer's sample count at +0x64c.
    u32 GetRendererSampleCount() const { return mRendererParameter.sampleCount; }
    OutputMode GetOutputMode() const { return mOutputMode; }
    // Inline-only, name guessed: aal::FinalOutputMeasure::updateActivation 0x7100baa290 compares +0x648
    // with 48000.
    u32 GetRendererSampleRate() const { return mRendererParameter.sampleRate; }
    // 0x71013528ec / 0x71013528f4: the effect-aux list is guarded by the mutex at +0x368.
    void LockEffectAuxList();
    void UnlockEffectAuxList();
    // 0x710134617c: waits on the audio-renderer event at +0x88.
    bool TimedWaitAudioRendererEvent(nn::TimeSpan span);

private:
    // 0x71013443f0: native setup routine, declared only.
    void SetupAudioRendererParameter(nn::audio::AudioRendererParameter* renderer_parameter,
                                     const HardwareManagerParameter& parameter) const;

    bool mIsInitialized;
    u8 _1[0x18 - 1];
    nn::audio::AudioRendererConfig mAudioRendererConfig;
    u8 _70[0x88 - 0x70];
    /* 0x88 */ nn::os::SystemEventType mAudioRendererEvent;
    u8 _b8[0xd0 - 0xb8];
    OutputMode mOutputMode;
    u8 _d4[0xdc - 0xd4];
    f32 mMasterVolumeStart;
    f32 mMasterVolumeTarget;
    s32 mMasterVolumeFrames;
    s32 mMasterVolumeCurrentFrame;
    u8 _ec[0x368 - 0xec];
    /* 0x368 */ nn::os::MutexType mEffectAuxMutex;
    u8 _388[0x648 - 0x388];
    nn::audio::AudioRendererParameter mRendererParameter;  // +0x648
    u8 _684[0x758 - 0x684];
    // +0x758: GetChannelCountMax returns 2 when it is set and 6 otherwise; what it means is not
    // established, so it keeps a placeholder name.
    u8 _758;
    u8 _759[7];
    nn::os::MutexType mAudioRendererMutex;  // +0x760
};

}  // namespace nn::atk::detail::driver
