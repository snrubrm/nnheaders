#pragma once

#include <nn/atk/OutputMode.h>
#include <nn/audio.h>
#include <nn/atk/detail/Util.h>
#include <nn/types.h>

namespace nn::atk::detail::driver {

class HardwareManager {
public:
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

    void SetMasterVolume(f32 volume, s32 frames);
    /// 0x71013450b8 (declared only)
    void SetOutputMode(OutputMode mode, OutputDevice device);
    OutputMode GetOutputMode() const { return mOutputMode; }

private:
    bool mIsInitialized;
    u8 _1[0x18 - 1];
    nn::audio::AudioRendererConfig mAudioRendererConfig;
    u8 _70[0xd0 - 0x70];
    OutputMode mOutputMode;
    u8 _d4[0xdc - 0xd4];
    f32 mMasterVolumeStart;
    f32 mMasterVolumeTarget;
    s32 mMasterVolumeFrames;
    s32 mMasterVolumeCurrentFrame;
};

}  // namespace nn::atk::detail::driver
