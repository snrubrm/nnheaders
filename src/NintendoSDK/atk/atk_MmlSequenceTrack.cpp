#include <nn/atk/detail/driver/MmlParser.h>
#include <nn/atk/detail/driver/MmlSequenceTrack.h>

namespace nn::atk::detail::driver {

int MmlSequenceTrack::Parse(bool flag) {
    return mParser->Parse(this, flag);
}

}  // namespace nn::atk::detail::driver
