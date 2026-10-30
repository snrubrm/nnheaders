#pragma once

#include <nn/atk/detail/driver/BasicSoundPlayer.h>
#include <nn/atk/detail/driver/Channel.h>
#include <nn/atk/detail/driver/DisposeCallback.h>
#include <nn/atk/detail/driver/SoundThread.h>
#include <nn/types.h>

namespace nn::atk::detail::driver {

// Partial player: the actual base subobjects and observed fields are modeled; lifetime remains
// a native dependency and the overall player size is not established here.
class WaveSoundPlayer : public BasicSoundPlayer,
                        public DisposeCallback,
                        public SoundThread::PlayerCallback {
public:
    WaveSoundPlayer();
    ~WaveSoundPlayer() override;
    void Initialize() override;
    void Finalize() override;
    void Start() override;
    void Stop() override;
    void Pause(bool pause) override;
    void InvalidateData(const void* begin, const void* end) override;
    void OnUpdateFrameSoundThread() override;
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency() override;
    void OnShutdownSoundThread() override;

    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool fix);
    s64 GetPlaySamplePosition(bool flag) const;
    static void ChannelCallbackFunc(Channel*, Channel::ChannelCallbackStatus, void*);

private:
    // Return type inferred from callers that discard the result.
    void Update();

    u8 _e0;
    bool mReleasePriorityFix;  // +0xe1
    u8 mChannelPriority;       // +0xe2
    u8 _e3[0x138 - 0xe3];
    Channel* mChannel;         // +0x138
    u32 _140;                  // copied from the prepare argument
};

}  // namespace nn::atk::detail::driver
