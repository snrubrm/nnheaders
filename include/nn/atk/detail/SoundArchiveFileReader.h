#pragma once

#include <nn/atk/SoundArchive.h>

namespace nn::atk::detail {

/// Reads the info block of a sound archive. Only the accessors that SoundArchive forwards
/// to are declared; the member layout is not modelled yet.
class SoundArchiveFileReader {
public:
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
};

}  // namespace nn::atk::detail
