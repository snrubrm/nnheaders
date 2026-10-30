#include <nn/atk/detail/PlayerHeap.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk::detail {

size_t PlayerHeap::GetAllocateSize(size_t size, bool withCallback) {
    if (withCallback)
        return nn::util::align_up(size + 0x20, 0x1000);
    return size;
}

}  // namespace nn::atk::detail
