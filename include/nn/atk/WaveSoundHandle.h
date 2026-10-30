#pragma once

namespace nn::atk {

namespace detail {
class WaveSound;
}

class WaveSoundHandle {
public:
    void DetachSound();

private:
    detail::WaveSound* mSound;
};

}  // namespace nn::atk
