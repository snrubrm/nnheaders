#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {

class SoundThread {
public:
    class PlayerCallback {
    public:
        PlayerCallback();
        virtual ~PlayerCallback();
        // Native dispatch ignores these results; void is inferred from the callback use.
        virtual void OnUpdateFrameSoundThread();
        virtual void OnUpdateFrameSoundThreadWithAudioFrameFrequency();
        virtual void OnShutdownSoundThread();

    private:
        nn::util::IntrusiveListNode mNode;
    };

    static SoundThread& GetInstance();

    /// Registers a function that is called by the sound thread every sound frame with `arg`.
    void RegisterSoundFrameUserCallback(void (*callback)(unsigned long), unsigned long arg);
    void ClearSoundFrameUserCallback();
    // 0x7101352740: the native body is empty.
    void ForceWakeup();
    void EffectFrameProcess();
};

static_assert(sizeof(SoundThread::PlayerCallback) == 0x18);

}  // namespace nn::atk::detail::driver
