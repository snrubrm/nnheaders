#include <nn/atk/detail/LowLevelVoice.h>
#include <nn/atk/detail/driver/HardwareManager.h>
#include <nn/atk/WaveBuffer.h>

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

}  // namespace nn::atk::detail
