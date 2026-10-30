#include <nn/atk/SoundSystem.h>
#include <nn/atk/detail/Util.h>
#include <nn/atk/detail/driver/HardwareManager.h>
#include <nn/atk/detail/driver/SoundThread.h>

namespace nn::atk {

// 0x7102662aa8: the flag IsInitialized returns and Finalize clears, both through the GOT.
bool sIsInitialized = false;

// SoundSystem initialization writes this native byte from SoundSystemParam +0x63.
u8 sUnk_710252D024 = 1;

void SoundSystem::VoiceCommandUpdate() {
    if (sUnk_710252D024)
        return;
    detail::driver::SoundThread::GetInstance().UpdateLowLevelVoices();
    detail::driver::SoundThread::GetInstance().EffectFrameProcess();
    detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
        .RequestUpdateAudioRenderer();
}

// NON_MATCHING: the compiler combines adjacent 32-bit defaults into 64-bit stores.
SoundSystem::SoundSystemParam::SoundSystemParam() {
    _0 = 0;
    _8 = 0x4000;
    _10 = 0x20000;
    _18 = 0x80000;
    _20 = 0;
    _50 = 0;
    _51 = 0;
    _52 = 0;
    _60 = 0;
    _61 = 0;
    _58 = 0;
    _6c = 0;
    _68 = 0;
    _69 = 0;
    _6a = 0;
    _6b = 0;
    _28 = 0x4000;
    _30 = 0x2000;
    mSampleRate = 48000;
    mEffectCount = 10;
    mVoiceCount = 96;
    _4c = 512;
    mRecorderFrameCount = 8;
    _66 = 1;
    _67 = 1;
    _62 = 1;
    _63 = 1;
    _64 = 1;
    _65 = 1;
}

int SoundSystem::GetRendererChannelCountMax() {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().GetChannelCountMax();
}

bool SoundSystem::IsClearEffectFinished(AuxBus bus, OutputDevice device) {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().IsFinishedClearEffect(
        bus, device, 0);
}

// 0x710133e244
int SoundSystem::GetRendererSampleCount() {
    return detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance()
        .GetRendererSampleCount();
}

// 0x710133dd80
bool SoundSystem::IsInitialized() {
    return sIsInitialized;
}

void SoundSystem::AttachMemoryPool(nn::audio::MemoryPoolType* pool, void* address, size_t size) {
    detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().AttachMemoryPool(
        pool, address, size, true);
}

void SoundSystem::DetachMemoryPool(nn::audio::MemoryPoolType* pool) {
    detail::Util::Singleton<detail::driver::HardwareManager>::GetInstance().DetachMemoryPool(pool, true);
}

}  // namespace nn::atk
