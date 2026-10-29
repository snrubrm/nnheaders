#pragma once

#include <nn/types.h>

namespace nn::atk {

namespace detail {
class SequenceSound;
}

// Partial model: the attached sound pointer and the detach used by the runtime sound instance.
class SequenceSoundHandle {
public:
    // 0x7101364cbc: clears the sound's temporary special handle when it points at this handle.
    void DetachSound();

    bool IsAttachedSound() const { return m_pSound != nullptr; }

private:
    detail::SequenceSound* m_pSound;  // +0
};

}  // namespace nn::atk
