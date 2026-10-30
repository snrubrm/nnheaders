#pragma once

#include <nn/atk/detail/VoiceInfo.h>
#include <nn/types.h>

namespace nn::atk::detail {

class LowLevelVoice;

class VirtualVoiceManager {
public:
    static VirtualVoiceManager& GetInstance();
    void Initialize();
    void UpdateVoiceInfo();
    int GetUnreleasedLowLevelVoiceCount() const;
    // Inline-only, name guessed: Voice::IsAvailable 1358bd8 and command dispatch 1359620
    // both index the pointer array at +0x28.
    LowLevelVoice* GetLowLevelVoice(u32 index) const { return mVoices[index]; }

private:
    // Inline-only: GetInstance 0x7101358c34 and Initialize 0x7101359378 repeat this reset.
    VirtualVoiceManager() { Initialize(); }

    u64 _0[4];
    u32 _20;
    u8 _24[4];
    LowLevelVoice* mVoices[256];        // +0x28
    VoiceInfo mVoiceInfo[2][256];       // +0x828
    u64 _4828[2][4];
};
static_assert(sizeof(VirtualVoiceManager) == 0x4868);

}  // namespace nn::atk::detail
