#include <nn/atk/detail/driver/Channel.h>

namespace nn::atk::detail::driver {

void Channel::DetachChannel(Channel* channel) {
    channel->mCallback = nullptr;
    channel->mCallbackArg = nullptr;
}

void Channel::SetBiquadFilter(int type, float frequency) {
    mBiquadType = type;
    mBiquadFrequency = frequency;
}

}  // namespace nn::atk::detail::driver
