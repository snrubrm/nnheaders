#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/SoundArchiveFileReader.h>

namespace nn::atk {

SoundArchive::SoundArchive() : mFileReader(nullptr), _10(nullptr), mFileVersion(0x2f) {}

// NON_MATCHING: the compiler inlines SoundArchive's constructor.
MemorySoundArchive::MemorySoundArchive() : mData(nullptr) {}

// NON_MATCHING: the compiler inlines SoundArchive::Initialize.
bool MemorySoundArchive::Initialize(const void* data) {
    mReader.Initialize(data);
    SoundArchive::Initialize(&mReader);
    const auto* bytes = static_cast<const u8*>(data);
    mReader.SetInfoBlock(bytes + mReader.GetInfoBlockOffset());
    const s32 string_offset = mReader.GetStringBlockOffset();
    if (mReader.GetStringBlockSize() != 0xffffffffu)
        mReader.SetStringBlock(bytes + string_offset);
    mData = data;
    return true;
}

// NON_MATCHING: the compiler inlines SoundArchive::Finalize.
void MemorySoundArchive::Finalize() {
    mData = nullptr;
    mReader.Finalize();
    SoundArchive::Finalize();
}

// The original destructor only restores the vtable; the reader is owned by Initialize/Finalize.
SoundArchive::~SoundArchive() {}

void SoundArchive::Initialize(detail::SoundArchiveFileReader* reader) {
    mFileReader = reader;
    mFileBlockOffset = reader->GetFileBlockOffset();
}

bool SoundArchive::IsAvailable() const {
    return mFileReader != nullptr;
}

// The file-system archive has no stream buffer of its own.
size_t FsSoundArchive::detail_GetRequiredStreamBufferSize() const {
    return 8;
}

u32 SoundArchive::GetSoundCount() const {
    return mFileReader->GetSoundCount();
}

u32 SoundArchive::GetPlayerCount() const {
    return mFileReader->GetPlayerCount();
}

u32 SoundArchive::GetGroupCount() const {
    return mFileReader->GetGroupCount();
}

u32 SoundArchive::GetWaveArchiveCount() const {
    return mFileReader->GetWaveArchiveCount();
}

u32 SoundArchive::detail_GetFileCount() const {
    return mFileReader->GetFileCount();
}

u32 SoundArchive::GetItemFileId(u32 item_id) const {
    return mFileReader->GetItemFileId(item_id);
}

u32 SoundArchive::GetItemPrefetchFileId(u32 item_id) const {
    return mFileReader->GetItemPrefetchFileId(item_id);
}

u32 SoundArchive::GetSoundType(u32 sound_id) const {
    return mFileReader->GetSoundType(sound_id);
}

bool SoundArchive::detail_ReadAdvancedWaveSoundInfo(u32 sound_id,
                                                    AdvancedWaveSoundInfo* info) const {
    return mFileReader->ReadAdvancedWaveSoundInfo(sound_id, info);
}

const u32* SoundArchive::detail_GetWaveArchiveIdTable(u32 wave_archive_id) const {
    return mFileReader->GetWaveArchiveIdTable(wave_archive_id);
}

// 0x7101339f40
void SoundArchive::Finalize() {
    if (mFileReader) {
        mFileReader->Finalize();
        mFileReader = nullptr;
    }
    mFileVersion = 0x2f;
}

// 0x7101339ecc: the memory archive has no external streams.
detail::fnd::FileStream* MemorySoundArchive::OpenExtStream(void*, size_t, const char*, void*,
                                                  size_t) const {
    return nullptr;
}

// 0x7101339ed4: the 0x20-byte file stream object is the whole buffer requirement.
size_t MemorySoundArchive::detail_GetRequiredStreamBufferSize() const {
    return 0x20;
}

}  // namespace nn::atk

