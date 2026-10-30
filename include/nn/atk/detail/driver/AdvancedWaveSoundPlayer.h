#pragma once

#include <nn/atk/detail/driver/BasicSoundPlayer.h>
#include <nn/atk/detail/driver/SoundThread.h>
#include <nn/types.h>

namespace nn::atk::detail::driver {

// Partial player: the real bases and observed state are modeled; lifetime remains a native
// dependency and the overall player size is not established here.
class AdvancedWaveSoundPlayer : public BasicSoundPlayer, public SoundThread::PlayerCallback {
public:
    AdvancedWaveSoundPlayer();
    ~AdvancedWaveSoundPlayer() override;
    void Initialize() override;
    void Finalize() override;
    void Start() override;
    void Stop() override;
    void Pause(bool pause) override;
    void OnUpdateFrameSoundThread() override;
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency() override;
    void OnShutdownSoundThread() override;

private:
    // UpdateTracks returns a numeric 0/1 status in native code.
    bool UpdateTracks();

    u8 _c8[0x758 - 0xc8];
    u32 _758;
    u8 _75c[0x764 - 0x75c];
    u8 _764;
    u8 _765;
    u8 _766;  // callback registration state
};

}  // namespace nn::atk::detail::driver
