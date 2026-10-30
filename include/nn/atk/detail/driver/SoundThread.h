#pragma once

#include <nn/atk/detail/fnd/Thread.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {

// Partial native model: the Handler base and callback-list root are established.
class SoundThread : public fnd::Thread::Handler {
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
        friend class SoundThread;
        nn::util::IntrusiveListNode mNode;
    };

    SoundThread();
    ~SoundThread() override;
    void Run(void* arg) override;
    static SoundThread& GetInstance();
    void RegisterPlayerCallback(PlayerCallback* callback);
    // Return type inferred from callers that discard the result.
    void UnregisterPlayerCallback(PlayerCallback* callback);

    /// Registers a function that is called by the sound thread every sound frame with `arg`.
    void RegisterSoundFrameUserCallback(void (*callback)(unsigned long), unsigned long arg);
    void ClearSoundFrameUserCallback();
    // 0x7101352740: the native body is empty.
    void ForceWakeup();
    void EffectFrameProcess();

private:
    fnd::Thread mThread;  // +0x8
    u8 _1f8[0x398 - 0x1f8];
    nn::util::detail::IntrusiveListImplementation mPlayerCallbacks;  // +0x398
};

static_assert(sizeof(fnd::Thread) == 0x1f0);
static_assert(sizeof(SoundThread::PlayerCallback) == 0x18);

}  // namespace nn::atk::detail::driver
