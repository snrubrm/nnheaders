#include <nn/atk/detail/AdvancedWaveSound.h>

namespace nn::atk::detail {

bool AdvancedWaveSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

}  // namespace nn::atk::detail
