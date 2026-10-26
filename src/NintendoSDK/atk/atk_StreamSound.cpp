#include <nn/atk/detail/StreamSound.h>

namespace nn::atk::detail {

bool StreamSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

}  // namespace nn::atk::detail
