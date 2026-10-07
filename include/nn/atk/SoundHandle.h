/**
 * @file SoundHandle.h
 * @brief Handle to a playing sound.
 */

#pragma once

#include <nn/types.h>

namespace nn::atk {

namespace detail {
class BasicSound;
}

/// A handle to a playing sound (the original header's SoundHandle). TODO: partial.
class SoundHandle {
public:
    SoundHandle() : m_pSound(nullptr) {}

    /// 0x71033c3b0 (declared only)
    void DetachSound();

    detail::BasicSound* m_pSound;
};

}  // namespace nn::atk
