/**
 * @file SoundArchiveManager.h
 * @brief Sound archive manager implementation.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace atk {
class SoundHandle;
class SoundArchive;
class SoundDataManager;

namespace detail {
class AddonSoundArchiveContainer;

/// The sound archive (and the data manager) that a SoundArchivePlayer plays from (0x40 bytes).
class SoundArchiveManager {
public:
    SoundArchiveManager();
    ~SoundArchiveManager();

    void Initialize(nn::atk::SoundArchive const*, nn::atk::SoundDataManager const*);
    void ChangeTargetArchive(char const*);
    void Finalize();
    bool IsAvailable() const;
    nn::atk::detail::AddonSoundArchiveContainer* GetAddonSoundArchive(char const*) const;

    const nn::atk::SoundArchive* mSoundArchive;
    const nn::atk::SoundDataManager* mSoundDataManager;
    /// The list of the addon archives (a node that points to itself when the list is empty).
    u64* _10;
    u64* _18;
    /// The archive that is used (the main archive or the one of an addon).
    const nn::atk::SoundArchive* mTargetSoundArchive;
    const nn::atk::SoundDataManager* mTargetSoundDataManager;
    u32 mSoundPlayerCount;  // read through SoundArchivePlayer::GetSoundPlayerCount
    u32 _34;
    u64 _38;
};
static_assert(sizeof(SoundArchiveManager) == 0x40);
}  // namespace detail
}  // namespace atk
}  // namespace nn
