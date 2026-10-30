#include <nn/atk/detail/driver/Channel.h>
#include <nn/atk/detail/driver/MultiVoice.h>

namespace nn::atk::detail::driver {

void Channel::Stop() {
    if (mVoice) {
        mVoice->Stop();
        mVoice->Free();
        mVoice = nullptr;
        mPaused = 0;
        mActive = 0;
    }
}

void Channel::UpdateSweep(int milliseconds) {
    _134 += milliseconds;
    if (_134 > _138)
        _134 = _138;
}

void Channel::SetSweepParam(float value, int duration, bool flag) {
    _130 = value;
    _138 = duration;
    _c3 = flag;
    _134 = 0;
}

void Channel::CallChannelCallback(ChannelCallbackStatus status) {
    if (mCallback) {
        mCallback(this, status, mCallbackArg);
        mCallback = nullptr;
        mCallbackArg = nullptr;
    }
}

void Channel::DetachChannel(Channel* channel) {
    channel->mCallback = nullptr;
    channel->mCallbackArg = nullptr;
}

void Channel::SetBiquadFilter(int type, float frequency) {
    mBiquadType = type;
    mBiquadFrequency = frequency;
}

}  // namespace nn::atk::detail::driver
