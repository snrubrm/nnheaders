#include <nn/atk/detail/Util.h>

namespace nn::atk::detail::Util {

// NON_MATCHING: format-case ordering and return-block placement differ.
size_t GetByteBySample(size_t sampleCount, SampleFormat format) {
    if (static_cast<int>(format) == 0)
        return sampleCount;
    if (static_cast<int>(format) == 1)
        return sampleCount * 2;
    if (static_cast<int>(format) != 2)
        return 0;

    size_t byteCount = (sampleCount / 14) * 8;
    const size_t remainingSamples = sampleCount % 14;
    if (remainingSamples != 0)
        byteCount += 1 + (remainingSamples + 1) / 2;
    return byteCount;
}

bool IsValidMemoryForDsp(const void*, size_t) {
    // The native target implementation accepts every memory range.
    return true;
}

// NON_MATCHING: the overflow record is emitted as a separate block rather than a loop.
void WarningLogger::Log(int type, int arg1, int arg2) {
    if (!mCurrent || mCurrent->mCount >= 64)
        return;
    Entry& entry = mCurrent->mEntries[mCurrent->mCount];
    entry.mType = type;
    entry.mArg1 = arg1;
    entry.mArg2 = arg2;
    ++mCurrent->mCount;
    if (mCurrent->mCount == 63) {
        Entry& overflow = mCurrent->mEntries[mCurrent->mCount];
        overflow.mType = 2;
        overflow.mArg1 = -1;
        overflow.mArg2 = -1;
        ++mCurrent->mCount;
    }
}

void WarningLogger::SwapBuffer() {
    Buffer* next;
    if (mCurrent == &mBuffers[0])
        next = &mBuffers[1];
    else if (mCurrent == &mBuffers[1])
        next = &mBuffers[0];
    else
        return;
    next->mCount = 0;
    mCurrent = next;
}

// NON_MATCHING: the compiler emits separate stores for the two buffers.
void WarningLogger::Print() {
    if (mCurrent == &mBuffers[0])
        mBuffers[1].mCount = 0;
    else if (mCurrent == &mBuffers[1])
        mBuffers[0].mCount = 0;
}

}  // namespace nn::atk::detail::Util
