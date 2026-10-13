#include <nn/atk/SoundHeap.h>
#include <nn/util/util_BitUtil.h>

namespace nn::atk {

size_t SoundHeap::GetAllocateSize(size_t size, bool align) {
    return align ? nn::util::align_up(size, 0x1000) : size;
}

}  // namespace nn::atk
