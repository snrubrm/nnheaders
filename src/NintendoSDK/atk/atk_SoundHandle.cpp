#include <nn/atk/SoundHandle.h>
#include <nn/atk/detail/BasicSound.h>

namespace nn::atk {

void SoundHandle::DetachSound() {
    if (IsAttachedSound()) {
        if (m_pSound->mGeneralHandle == this)
            m_pSound->mGeneralHandle = nullptr;
        if (m_pSound->mTempGeneralHandle == this)
            m_pSound->mTempGeneralHandle = nullptr;
    }
    if (m_pSound != nullptr)
        m_pSound = nullptr;
}

void SoundHandle::detail_AttachSound(detail::BasicSound* sound) {
    m_pSound = sound;
    if (m_pSound->IsAttachedGeneralHandle())
        m_pSound->DetachGeneralHandle();
    m_pSound->mGeneralHandle = this;
}

}  // namespace nn::atk
