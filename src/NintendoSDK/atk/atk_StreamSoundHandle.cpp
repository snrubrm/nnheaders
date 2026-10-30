#include <nn/atk/StreamSoundHandle.h>
#include <nn/atk/detail/StreamSound.h>

namespace nn::atk {

void StreamSoundHandle::DetachSound() {
    if (mSound) {
        if (mSound->mTempSpecialHandle == this)
            mSound->mTempSpecialHandle = nullptr;
    }
    if (mSound)
        mSound = nullptr;
}

}  // namespace nn::atk
