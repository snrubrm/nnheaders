/**
 * @file SoundStartable.h
 * @brief Interface to start sounds.
 */

#pragma once

#include <nn/types.h>
#include <vapours/results.hpp>

namespace nn::atk {

class SoundHandle;

/// TODO: only the functions that sead::AudioPlayerCafe uses are declared.
class SoundStartable {
public:
    /// The information a sound is started with (the fields are not known).
    struct StartInfo;

    virtual ~SoundStartable();

    /// Sets the handle up to play the sound `sound_id`.
    virtual Result detail_SetupSound(SoundHandle* handle, u32 sound_id, bool hold, const char* label,
                                     const StartInfo* start_info) = 0;
    virtual u32 detail_GetItemId(const char* label) = 0;
    virtual u32 detail_GetItemId(const char* label, const char* name) = 0;

    /// 0x133cfbc / 0x133d00c (declared only)
    Result StartSound(SoundHandle* handle, u32 sound_id, const StartInfo* start_info);
    Result StartSound(SoundHandle* handle, const char* label, const StartInfo* start_info);
    Result HoldSound(SoundHandle* handle, u32 sound_id, const StartInfo* start_info);
    Result HoldSound(SoundHandle* handle, const char* label, const StartInfo* start_info);
};

}  // namespace nn::atk
