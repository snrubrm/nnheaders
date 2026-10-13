#include <nn/atk/SoundStartable.h>
#include <nn/atk/SoundHandle.h>
#include <nn/atk/detail/BasicSound.h>

namespace nn::atk {

Result SoundStartable::StartSound(SoundHandle* handle, u32 sound_id, const StartInfo* start_info) {
    Result result = detail_SetupSound(handle, sound_id, false, nullptr, start_info);
    if (result.IsFailure())
        return result;
    if (handle->m_pSound)
        handle->m_pSound->StartPrepared();
    return ResultSuccess();
}

Result SoundStartable::HoldSound(SoundHandle* handle, u32 sound_id, const StartInfo* start_info) {
    return HoldSound(handle, sound_id, nullptr, start_info);
}

Result SoundStartable::PrepareSound(SoundHandle* handle, u32 sound_id, const StartInfo* start_info) {
    return detail_SetupSound(handle, sound_id, false, nullptr, start_info);
}

}  // namespace nn::atk
