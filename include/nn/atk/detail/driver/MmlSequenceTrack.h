#pragma once

#include <nn/atk/detail/driver/SequenceTrack.h>

namespace nn::atk::detail::driver {

class MmlParser;

class MmlSequenceTrack : public SequenceTrack {
public:
    MmlSequenceTrack();
    ~MmlSequenceTrack() override;
    int Parse(bool flag) override;

private:
    MmlParser* mParser;  // +0x1e8
};
static_assert(sizeof(MmlSequenceTrack) == 0x1f0);

}  // namespace nn::atk::detail::driver
