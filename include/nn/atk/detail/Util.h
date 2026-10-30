#pragma once

#include <nn/types.h>
#include <nn/util/util_BytePtr.h>
#include <nn/atk/SampleFormat.h>

namespace nn::atk::detail {

namespace Util {

size_t GetByteBySample(size_t sampleCount, SampleFormat format);
bool IsValidMemoryForDsp(const void* memory, size_t size);
u32 CalcRandom();

/// A typed reference inside a resource block: the item type and its offset from the reference's
/// base (the containing table or structure).
struct Reference {
    u16 typeId;
    u16 padding;
    s32 offset;
};

/// A count followed by that many references, each relative to the table.
struct ReferenceTable {
    u32 count;
    Reference item[1];

    // inline-only in the original; name is a guess. The table lookup repeats in
    // BankFile::InfoBlockBody::GetInstrument and WaveSoundFile::InfoBlockBody::GetWaveSoundData.
    const void* GetReferedItem(u32 index) const {
        if (index >= count)
            return nullptr;
        return nn::util::ConstBytePtr(this, item[index].offset).Get();
    }

    const void* GetReferedItem(u32 index, u16 typeId) const {
        if (index >= count)
            return nullptr;
        if (item[index].typeId != typeId)
            return nullptr;
        return nn::util::ConstBytePtr(this, item[index].offset).Get();
    }
};

/// A block reference of a resource file header: the block type, its offset from the file header
/// (0 when absent) and its size.
struct ReferenceWithSize {
    u16 typeId;
    u16 padding;
    s32 offset;
    u32 size;
};

/// inline-only in the original; name is a guess. Returns the first block of the given type, or null;
/// every resource FileHeader::Get*Block (e.g. WaveFile 0x7101364ed8) is this search over the
/// header's block count at +0x10 and its references at +0x14.
inline const void* FindBlock(const void* fileHeader, u16 blockCount,
                             const ReferenceWithSize* blocks, u16 typeId) {
    const ReferenceWithSize* ref = nullptr;
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].typeId == typeId) {
            ref = &blocks[i];
            break;
        }
    }
    if (ref == nullptr)
        return nullptr;
    if (ref->offset == 0)
        return nullptr;
    return nn::util::ConstBytePtr(fileHeader, ref->offset).Get();
}

/// inline-only in the original; name is a guess. Reads a resource option parameter: a flag word followed by one 32-bit
/// value per set flag, in bit order. Always inlined in the original; this body reproduces the
/// counting loop of SoundArchiveFile::SoundInfo::IsFrontBypass (0x7101340d84) and BankFile::VelocityRegion::IsIgnoreNoteOff (0x71013603e0), which is not
/// unrolled there.
template <typename T>
inline bool ReadOptionParameter(const u32* option, int bit, T* value) {
    bool found = false;
    u32 count = 0;
    for (int i = 0; i <= bit; ++i) {
        if (*option & (1u << i)) {
            ++count;
            if (i == bit)
                found = true;
        }
    }
    if (count == 0 || !found)
        return false;
    *value = *nn::util::ConstBytePtr(option, count * sizeof(u32)).Get<T>();
    return true;
}

class WarningLogger {
public:
    WarningLogger() : mCurrent(&mBuffers[0]) {}
    void Log(int type, int arg1, int arg2);
    void SwapBuffer();
    void Print();

private:
    struct Entry {
        int mType;
        int mArg1;
        int mArg2;
    };
    struct Buffer {
        Entry mEntries[64];
        int mCount = 0;
    };
    Buffer mBuffers[2];
    Buffer* mCurrent;
};
static_assert(sizeof(WarningLogger) == 0x610);

/// Lazily constructed global instance (a function-local static in the sound library).
template <typename T>
class Singleton {
public:
    static T& GetInstance();
};

}  // namespace Util

}  // namespace nn::atk::detail
