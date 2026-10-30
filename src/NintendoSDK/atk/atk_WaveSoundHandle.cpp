#include <nn/atk/WaveSoundHandle.h>
#include <nn/atk/detail/WaveSound.h>

namespace nn::atk {

void WaveSoundHandle::DetachSound() {
    if (mSound) {
        if (mSound->mTempSpecialHandle == this)
            mSound->mTempSpecialHandle = nullptr;
    }
    if (mSound)
        mSound = nullptr;
}

}  // namespace nn::atk
