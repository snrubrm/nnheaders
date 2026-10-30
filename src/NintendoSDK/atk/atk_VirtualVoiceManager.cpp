#include <nn/atk/detail/VirtualVoiceManager.h>
#include <nn/atk/detail/LowLevelVoice.h>

namespace nn::atk::detail {

// NON_MATCHING: the compiler schedules the initial scalar stores in a different order.
VirtualVoiceManager& VirtualVoiceManager::GetInstance() {
    static VirtualVoiceManager sUnk_7102663EF0;
    return sUnk_7102663EF0;
}

void VirtualVoiceManager::Initialize() {
    for (u64& word : _0)
        word = ~0ULL;
    for (auto& buffer : _4828)
        for (u64& word : buffer)
            word = ~0ULL;
    _20 = 0;
    for (int i = 0; i < 256; ++i) {
        mVoices[i] = nullptr;
        mVoiceInfo[0][i]._0 = 1;
        mVoiceInfo[1][i]._0 = 1;
    }
}

// NON_MATCHING: the compiler allocates an extra saved register and orders the bitset paths
// differently.
void VirtualVoiceManager::UpdateVoiceInfo() {
    const u32 buffer = _20 == 0;
    for (u32 i = 0; i < 256; ++i) {
        VoiceInfo& info = mVoiceInfo[buffer][i];
        u64& bits = _4828[buffer][i / 64];
        const u64 mask = 1ULL << (i % 64);
        if (mVoices[i]) {
            mVoices[i]->UpdateVoiceInfo(&info);
            bits |= mask;
        } else if (bits & mask) {
            info._0 = 1;
            info.mWaveBufferState = 0;
            info.mVoice = nullptr;
            bits &= ~mask;
        }
    }
    _20 = buffer;
}

// NON_MATCHING: the compiler unrolls the pointer-counting loop four entries at a time.
int VirtualVoiceManager::GetUnreleasedLowLevelVoiceCount() const {
    int count = 0;
    for (const LowLevelVoice* voice : mVoices) {
        if (voice)
            ++count;
    }
    return count;
}

}  // namespace nn::atk::detail
