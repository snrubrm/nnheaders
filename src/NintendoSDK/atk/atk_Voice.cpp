#include <nn/atk/detail/Voice.h>

namespace nn::atk::detail {

// 0x7101358bd4
Voice::~Voice() {}

Voice::Voice() {
    _ac = -1;
}

u64 Voice::GetPlayPosition() const {
    return mPlayPosition;
}

}  // namespace nn::atk::detail
