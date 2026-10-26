#include <nn/atk/detail/SequenceSound.h>

namespace nn::atk::detail {

bool SequenceSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != nullptr;
}

}  // namespace nn::atk::detail
