#pragma once

#include <nn/types.h>

namespace nn::atk {

/// How the sound is mixed to the speakers. The names are from the sound library documentation, the values follow the
/// table that sead::AudioSystemCafe converts with (not verified).
enum OutputMode : s32 {
    OutputMode_Surround = 0,
    OutputMode_Stereo = 1,
    OutputMode_Mono = 2,
};

/// The output devices (the Switch only has the main one).
enum OutputDevice : s32 {
    OutputDevice_Main = 0,
};

}  // namespace nn::atk
