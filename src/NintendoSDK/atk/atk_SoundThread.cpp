#include <nn/atk/detail/driver/SoundThread.h>
#include <nn/atk/detail/driver/HardwareManager.h>
#include <nn/atk/detail/CommandManager.h>
#include <nn/atk/detail/LowLevelVoiceCommand.h>
#include <nn/atk/detail/VirtualVoiceManager.h>

namespace nn::atk::detail::driver {

// NON_MATCHING: the singleton mutex address is obtained from the returned pointer.
void SoundThread::RegisterSoundFrameUserCallback(void (*callback)(unsigned long), unsigned long arg) {
    nn::os::MutexType* mutex = &GetInstance().mUserCallbackMutex;
    nn::os::LockMutex(mutex);
    mUserCallbackArg = arg;
    mUserCallback = callback;
    GetInstance();
    nn::os::UnlockMutex(mutex);
}

// NON_MATCHING: singleton address materialization and callback-field store order differ.
void SoundThread::ClearSoundFrameUserCallback() {
    nn::os::MutexType* mutex = &GetInstance().mUserCallbackMutex;
    nn::os::LockMutex(mutex);
    mUserCallback = nullptr;
    mUserCallbackArg = 0;
    GetInstance();
    nn::os::UnlockMutex(mutex);
}

void SoundThread::UpdateLowLevelVoices() {
    LowLevelVoiceCommand::GetInstance().ProcessCommand();
    const OutputMode mode = Util::Singleton<HardwareManager>::GetInstance().GetOutputMode();
    Util::Singleton<HardwareManager>::GetInstance().GetLowLevelVoiceAllocator().UpdateAllVoiceState(mode);
    VirtualVoiceManager::GetInstance().UpdateVoiceInfo();
}

// NON_MATCHING: the normalized Run result is stored from w8 instead of w0.
void SoundThread::CreateSoundThread(int priority, void* stack, size_t stackSize, int core,
                                    u32 affinity) {
    _3d8 = affinity;
    fnd::Thread::RunArgs args;
    args.name = "nn::atk::detail::driver::SoundThread";
    args.stack = stack;
    args.stackSize = stackSize;
    args.core = core;
    args.affinity = static_cast<fnd::Thread::AffinityMask>(affinity);
    args.priority = priority;
    args.argument = nullptr;
    args.handler = this;
    _3dc = mThread.Run(args);
}

void SoundThread::Initialize(void* buffer, size_t size, bool enabled, bool) {
    _414 = enabled;
    mPerformanceReader = nullptr;
    mPerformanceBufferIndex = 0;
    _4c0.store(0, std::memory_order_release);

    const size_t part_size = size / 3;
    auto* first = static_cast<u8*>(buffer);
    auto* second = first + part_size;
    mPerformanceBufferSize = part_size;
    mPerformanceBuffers[0] = first;
    mPerformanceBuffers[1] = second;
    mPerformanceBuffers[2] = second + part_size;
    VirtualVoiceManager::GetInstance().Initialize();
}

void SoundThread::Finalize() {
    for (auto it = mPlayerCallbacks.begin(); it != mPlayerCallbacks.end();) {
        PlayerCallback& callback = *it++;
        callback.OnShutdownSoundThread();
    }
    mPerformanceReader = nullptr;
    mPerformanceBufferIndex = 0;
    mPerformanceBufferSize = 0;
    mPerformanceBuffers[2] = nullptr;
    mPerformanceBuffers[1] = nullptr;
    mPerformanceBuffers[0] = nullptr;
}

// NON_MATCHING: the native Join call passes through a separate branch thunk.
void SoundThread::Destroy() {
    if (_3dc) {
        nn::os::JamMessageQueue(&mQueue, 0x20000000);
        mThread.Join();
        mThread.Release();
        _3d8 = 0;
        _3dc = 0;
    }
}

void SoundThread::RegisterPlayerCallback(PlayerCallback* callback) {
    mPlayerCallbacks.push_back(*callback);
}

void SoundThread::UnregisterPlayerCallback(PlayerCallback* callback) {
    mPlayerCallbacks.erase(mPlayerCallbacks.iterator_to(*callback));
}

void SoundThread::ForceWakeup() {
}

void SoundThread::EffectFrameProcess() {
    Util::Singleton<HardwareManager>::GetInstance().UpdateEffect();
}

}  // namespace nn::atk::detail::driver
