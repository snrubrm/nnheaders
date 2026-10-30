#include <nn/atk/detail/LowLevelVoice.h>
#include <nn/atk/detail/driver/HardwareManager.h>
#include <nn/atk/WaveBuffer.h>
#include <nn/atk/detail/VoiceInfo.h>

namespace nn::atk::detail {

LowLevelVoiceAllocator::LowLevelVoiceAllocator()
    : mVoices(nullptr), mVoiceOrder(nullptr), mActiveVoiceCount(0), mVoiceIndices(nullptr),
      mVoiceCapacity(0), _24(0) {
}

bool LowLevelVoice::IsAvailable() const {
    return mAvailable;
}

void LowLevelVoice::SetPriority(int priority) {
    mPriority = priority;
    if (nn::audio::IsVoiceValid(&mVoice))
        nn::audio::SetVoicePriority(&mVoice, mPriority == 255 ? 0 : 255 - mPriority);
}

// NON_MATCHING: the compiler places the acquire branch after the release branch.
void LowLevelVoice::SetState(VoiceState state) {
    mState = state;
    if (state == VoiceState_Released) {
        if (mHasVoiceSlot) {
            auto& manager = Util::Singleton<driver::HardwareManager>::GetInstance();
            nn::audio::ReleaseVoiceSlot(manager.GetAudioRendererConfig(), &mVoice);
            mHasVoiceSlot = false;
        }
    } else if (state == VoiceState_Acquired && !mHasVoiceSlot) {
        AllocVoice();
        mHasVoiceSlot = true;
    }
}

// NON_MATCHING: the compiler swaps the two registers used to choose the list link.
void LowLevelVoice::AppendWaveBuffer(WaveBuffer* buffer) {
    buffer->mNext = nullptr;
    buffer->_34 = 1;
    if (mWaveBufferTail)
        mWaveBufferTail->mNext = buffer;
    else
        mWaveBufferHead = buffer;
    mWaveBufferTail = buffer;
}

void LowLevelVoice::FreeAllWaveBuffer() {
    for (WaveBuffer* buffer = mWaveBufferHead; buffer; buffer = buffer->mNext)
        buffer->_34 = 3;
    mWaveBufferTail = nullptr;
    mWaveBufferCursor = nullptr;
    mWaveBufferHead = nullptr;
}

// NON_MATCHING: the compiler reloads the current buffer after writing the output state.
void LowLevelVoice::UpdateVoiceInfo(VoiceInfo* info) const {
    info->mVoice = mSourceVoice;
    info->_0 = mState;
    if (mWaveBufferHead) {
        info->mWaveBufferState = mWaveBufferHead->_34;
        info->mWaveBuffer = mWaveBufferHead->mSource;
    } else {
        info->mWaveBufferState = 0;
    }
    info->mPlayPosition = _b8;
}

// NON_MATCHING: the compiler combines the array sizes before multiplying the voice count.
size_t LowLevelVoiceAllocator::GetRequiredMemSize(int max_voices) {
    return max_voices * (sizeof(LowLevelVoice) + sizeof(LowLevelVoice*) + sizeof(s32)) + 0x40;
}

// NON_MATCHING: the compiler uses signed pointer-difference arithmetic and orders the loads
// differently; the selected voice belongs to the allocator's voice array.
LowLevelVoice* LowLevelVoiceAllocator::AllocVoice() {
    if (mActiveVoiceCount >= mVoiceCapacity)
        return nullptr;

    const s32 active_index = mActiveVoiceCount++;
    LowLevelVoice* voice = mVoiceOrder[active_index];
    mVoiceIndices[voice - mVoices] = active_index;
    voice->mPriority = 0;
    voice->_b8 = 0;
    voice->_c0 = 0;
    voice->_e8 = 0;
    voice->mAvailable = true;
    voice->mSampleRate = 32000;
    voice->_b0 = 1;
    voice->_ec = 1;
    return voice;
}

// NON_MATCHING: the compiler reloads allocator fields and uses signed pointer differences;
// the voices belong to the same initialized array.
void LowLevelVoiceAllocator::FreeVoice(LowLevelVoice* voice) {
    voice->FreeAllWaveBuffer();
    if (voice->mHasVoiceSlot) {
        auto& manager = Util::Singleton<driver::HardwareManager>::GetInstance();
        nn::audio::ReleaseVoiceSlot(manager.GetAudioRendererConfig(), &voice->mVoice);
        voice->mHasVoiceSlot = false;
    }
    voice->mSourceVoice = nullptr;
    voice->mAvailable = false;
    voice->mState = VoiceState_Released;

    const s32 index = mVoiceIndices[voice - mVoices];
    --mActiveVoiceCount;
    LowLevelVoice* last_voice = mVoiceOrder[mActiveVoiceCount];
    mVoiceOrder[index] = last_voice;
    mVoiceOrder[mActiveVoiceCount] = voice;
    mVoiceIndices[last_voice - mVoices] = index;
    mVoiceIndices[voice - mVoices] = -1;
}

}  // namespace nn::atk::detail
