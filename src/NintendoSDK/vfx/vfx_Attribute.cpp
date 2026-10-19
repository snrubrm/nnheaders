#include <nn/vfx/detail/Attribute.h>

namespace nn::vfx::detail {

Attribute::Attribute() = default;

void* Attribute::Map(BufferSide side) {
    return mMapped[u32(side)];
}

void Attribute::Unmap() {}

const void* Attribute::GetGpuAddress(BufferSide side) const {
    return &mBuffers[u32(side)];
}

}  // namespace nn::vfx::detail
