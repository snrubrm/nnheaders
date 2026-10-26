#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Partial models. The player handle is an opaque driver object; the sound classes only need its
// address for GetBasicSoundPlayerHandle, so its extent is not modelled. The base relationship
// follows the getter's return type.
class SoundPlayerHandle {};

class BasicSoundPlayerHandle : public SoundPlayerHandle {};

}  // namespace nn::atk::detail
