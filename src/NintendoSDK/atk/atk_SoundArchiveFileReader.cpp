#include <nn/atk/detail/SoundArchiveFileReader.h>

namespace nn {
namespace atk {
namespace detail {

u32 SoundArchiveFileReader::GetItemFileId(u32 item_id) const {
    return mInfoBlock->GetItemFileId(item_id);
}

u32 SoundArchiveFileReader::GetItemPrefetchFileId(u32 item_id) const {
    return mInfoBlock->GetItemPrefetchFileId(item_id);
}

u32 SoundArchiveFileReader::GetSoundType(u32 sound_id) const {
    const SoundArchiveFile::SoundInfo* info = mInfoBlock->GetSoundInfo(sound_id);
    if (!info)
        return 0;
    return info->GetSoundType();
}

u32 SoundArchiveFileReader::GetSoundCount() const {
    return *mInfoBlock->GetSoundInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetPlayerCount() const {
    return *mInfoBlock->GetPlayerInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetGroupCount() const {
    return *mInfoBlock->GetGroupInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetWaveArchiveCount() const {
    return *mInfoBlock->GetWaveArchiveInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetFileCount() const {
    return *mInfoBlock->GetFileInfoReferenceTable();
}

}  // namespace detail
}  // namespace atk
}  // namespace nn
