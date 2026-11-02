#pragma once

#include <nn/atk/detail/fnd/Thread.h>
#include <nn/os/os_Mutex.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/util/util_IntrusiveList.h>
#include <atomic>

namespace nn::atk {
class AudioRendererPerformanceReader;
}

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
    // Native caller discards the result; void is inferred.
    void CreateSoundThread(int priority, void* stack, size_t stackSize, int core, u32 affinity);
    // SoundSystem discards the native results; void is inferred.
    void Initialize(void* buffer, size_t size, bool enabled, bool unused);
    void Destroy();
    void RegisterPlayerCallback(PlayerCallback* callback);
    // Return type inferred from callers that discard the result.
    void UnregisterPlayerCallback(PlayerCallback* callback);

    /// Registers a function that is called by the sound thread every sound frame with `arg`.
    void RegisterSoundFrameUserCallback(void (*callback)(unsigned long), unsigned long arg);
    void ClearSoundFrameUserCallback();
    // 0x7101352740: the native body is empty.
    void ForceWakeup();
    void EffectFrameProcess();
    // Native caller discards the result; void is inferred.
    void UpdateLowLevelVoices();

private:
    fnd::Thread mThread;  // +0x8
    nn::os::MessageQueueType mQueue;  // +0x1f8
    u64 mQueueBuffer[32];             // +0x240
    u8 _340[0x348 - 0x340];
    nn::os::MutexType mUserCallbackMutex;  // +0x348
    u8 _368[0x398 - 0x368];
    nn::util::detail::IntrusiveListImplementation mPlayerCallbacks;  // +0x398
    void (*mUserCallback)(unsigned long);  // +0x3a8
    unsigned long mUserCallbackArg;          // +0x3b0
    u8 _3b8[0x3d8 - 0x3b8];
    u32 _3d8;
    u8 _3dc;
    u8 _3dd[0x3f0 - 0x3dd];
    u8* mPerformanceBuffers[3];  // +0x3f0
    size_t mPerformanceBufferSize;  // +0x408
    s32 mPerformanceBufferIndex;  // +0x410
    u8 _414;
    u8 _415[0x428 - 0x415];
    nn::atk::AudioRendererPerformanceReader* mPerformanceReader;  // +0x428
    u8 _430[0x4c0 - 0x430];
    std::atomic<u32> _4c0;
};

static_assert(sizeof(SoundThread) == 0x4c8);
static_assert(sizeof(fnd::Thread) == 0x1f0);
static_assert(sizeof(SoundThread::PlayerCallback) == 0x18);

}  // namespace nn::atk::detail::driver
