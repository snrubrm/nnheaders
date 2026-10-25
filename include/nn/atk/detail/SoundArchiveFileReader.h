#pragma once

#include <nn/atk/SoundArchive.h>

namespace nn::atk::detail {

/// Reads the info block of a sound archive. The file header copy at +0 gives the block offsets;
/// the info block pointer at +0x40 is what the count and record accessors use.
class SoundArchiveFileReader {
public:
    SoundArchiveFileReader();
    void Initialize(const void* file);
    void Finalize();
    void SetStringBlock(const void* block);
    void SetInfoBlock(const void* block);
    s32 GetItemId(const char* label) const;
    const char* GetItemLabel(u32 item_id) const;
    u32 GetItemFileId(u32 item_id) const;
    u32 GetItemPrefetchFileId(u32 item_id) const;
    u32 GetSoundCount() const;
    u32 GetPlayerCount() const;
    u32 GetGroupCount() const;
    u32 GetWaveArchiveCount() const;
    u32 GetFileCount() const;
    u32 GetSoundType(u32 sound_id) const;
    const u32* GetWaveArchiveIdTable(u32 wave_archive_id) const;
    bool ReadAdvancedWaveSoundInfo(u32 sound_id, SoundArchive::AdvancedWaveSoundInfo* info) const;

private:
    SoundArchiveFile::FileHeader mFileHeader;              // +0
    const SoundArchiveFile::StringBlockBody* mStringBlock;  // _38
    const SoundArchiveFile::InfoBlockBody* mInfoBlock;      // _40
    u64 _48;                                               // _48, zeroed with the two pointers
};

}  // namespace nn::atk::detail
