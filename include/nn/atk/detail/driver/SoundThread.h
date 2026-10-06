#pragma once

namespace nn::atk::detail::driver {

class SoundThread {
public:
    static SoundThread& GetInstance();

    /// Registers a function that is called by the sound thread every sound frame with `arg`.
    void RegisterSoundFrameUserCallback(void (*callback)(unsigned long), unsigned long arg);
    void ClearSoundFrameUserCallback();
};

}  // namespace nn::atk::detail::driver
