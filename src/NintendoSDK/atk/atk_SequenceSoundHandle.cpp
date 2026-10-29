#include <nn/atk/SequenceSoundHandle.h>
#include <nn/atk/detail/SequenceSound.h>

namespace nn::atk {

void SequenceSoundHandle::DetachSound() {
    if (IsAttachedSound()) {
        if (m_pSound->mTempSpecialHandle == this)
            m_pSound->mTempSpecialHandle = nullptr;
    }
    if (m_pSound != nullptr)
        m_pSound = nullptr;
}

}  // namespace nn::atk
