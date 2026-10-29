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

    /// 0x710133c3b0: clears the sound's general and temporary general handle slots that point
    /// at this handle.
    void DetachSound();
    /// 0x710133c3ec
    void detail_AttachSound(detail::BasicSound* sound);

    bool IsAttachedSound() const { return m_pSound != nullptr; }

    detail::BasicSound* m_pSound;
};

}  // namespace nn::atk
