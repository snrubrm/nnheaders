#include <nn/atk/detail/driver/MultiVoice.h>

namespace nn::atk::detail::driver {

SampleFormat MultiVoice::GetFormat() const {
    return mFormat;
}

s64 MultiVoice::GetCurrentPlayingSample() const {
    if (mVoiceCount < 1)
        return 0;
    return GetPlayPosition();
}

}  // namespace nn::atk::detail::driver
