#include <nn/atk/detail/WaveSound.h>

namespace nn::atk::detail {

bool WaveSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

}  // namespace nn::atk::detail
