/**
 * @file SoundArchivePlayer.h
 * @brief Basic sound player from a sound archive.
 */

#pragma once

#include <nn/atk/SoundArchive.h>
#include <nn/atk/SoundDataManager.h>
#include <nn/atk/SoundStartable.h>
#include <nn/atk/detail/SoundArchiveManager.h>

namespace nn {
namespace atk {
class SoundPlayer;

/// TODO: the sound runtimes (sequence, wave, advanced wave, stream) are not modeled.
class SoundArchivePlayer : public SoundStartable {
public:
    /// The arguments of Initialize (0x68 bytes; the fields that are not named are not known).
    struct InitializeParam {
        const SoundArchive* archive;
        const SoundDataManager* data_manager;
        void* setup_buffer;
        size_t setup_buffer_size;
        void* stream_buffer;
        size_t stream_buffer_size;
        void* stream_cache_buffer;
        size_t stream_cache_buffer_size;
        bool _40;
        void* _48;
        void* _50;
        u64 _58;
        u32 _60;

        InitializeParam() : _40(true), _48(nullptr), _50(nullptr), _58(0), _60(0) {}
    };
    static_assert(sizeof(InitializeParam) == 0x68);

    SoundArchivePlayer();
    ~SoundArchivePlayer() override;

    /// 0x133ab00
    bool IsAvailable() const;
    bool Initialize(const InitializeParam& param);
    void Finalize();
    void Update();
    void StopAllSound(s32, bool);
    void DisposeInstances();
    SoundPlayer* GetSoundPlayer(u32 player_id);
    u32 GetSoundPlayerCount() const { return mArchiveManager.mSoundPlayerCount; }
    const SoundArchive* GetSoundArchive() const;

    /// 0x133af1c / 0x133af24 / 0x133af34 (the first and the last are static)
    static size_t GetRequiredMemSize(const SoundArchive* archive, size_t unknown);
    u32 GetRequiredStreamBufferSize(const SoundArchive* archive) const;
    static size_t GetRequiredStreamCacheSize(const SoundArchive* archive, u32 unknown);

    Result detail_SetupSound(SoundHandle* handle, u32 sound_id, bool hold, const char* label,
                             const StartInfo* start_info) override;
    /// Inline in the original (0x7100bb97d4 / 0x7100bb97e4).
    u32 detail_GetItemId(const char* label) override { return detail_GetItemId(label, nullptr); }
    u32 detail_GetItemId(const char* label, const char* name) override {
        mArchiveManager.ChangeTargetArchive(name);
        return mArchiveManager.mTargetSoundArchive->GetItemId(label);
    }

    nn::atk::detail::SoundArchiveManager mArchiveManager;  // _8
    u8 _48[0x2a0 - 0x48];
    u8 mMemoryPool[8];  // _2a0 (nn::audio::MemoryPoolType)
    u8 _2a8[0x2c0 - 0x2a8];
};
static_assert(sizeof(SoundArchivePlayer) == 0x2c0);
}  // namespace atk
}  // namespace nn
