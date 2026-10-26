#include <nn/atk/detail/LowLevelVoice.h>

namespace nn::atk::detail {

bool LowLevelVoice::IsAvailable() const {
    return mAvailable;
}

}  // namespace nn::atk::detail
