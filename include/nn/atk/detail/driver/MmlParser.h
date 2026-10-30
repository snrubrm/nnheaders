#pragma once

#include <nn/types.h>

namespace nn::atk::detail::driver {

// The sequence runtime embeds this polymorphic parser at +0xa8. Its two command virtuals
// are not declared here until their return types are established.
class MmlParser {
public:
    virtual ~MmlParser();

    static u32 ParseAllocTrack(const void* data, u32 offset, u32* track_mask);
};
static_assert(sizeof(MmlParser) == 8);

}  // namespace nn::atk::detail::driver
