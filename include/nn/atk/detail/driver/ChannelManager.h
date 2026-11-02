#pragma once

namespace nn::atk::detail::driver {

class Channel;

// Only the named singleton and free operations used by Channel are declared.
class ChannelManager {
public:
    // The native singleton returns its manager address; reference return is inferred.
    static ChannelManager& GetInstance();
    // Channel::FreeChannel discards the native result; void is inferred.
    void Free(Channel* channel);
};

}  // namespace nn::atk::detail::driver
