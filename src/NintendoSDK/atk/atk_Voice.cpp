#include <nn/atk/detail/Voice.h>

namespace nn::atk::detail {

Voice::Voice() {
    _ac = -1;
}

u64 Voice::GetPlayPosition() const {
    return mPlayPosition;
}

}  // namespace nn::atk::detail
