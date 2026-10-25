#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/SoundArchiveFileReader.h>

namespace nn::atk {

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

}  // namespace nn::atk
