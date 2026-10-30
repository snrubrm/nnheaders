#include <nn/atk/detail/driver/MmlParser.h>

namespace nn::atk::detail::driver {

u32 MmlParser::ParseAllocTrack(const void* data, u32 offset, u32* track_mask) {
    const u8* command = static_cast<const u8*>(data) + offset;
    if (command[0] == 0xfe) {
        *track_mask = (command[1] << 8) | command[2] | 1;
        return offset + 3;
    }
    *track_mask = 1;
    return offset;
}

}  // namespace nn::atk::detail::driver
