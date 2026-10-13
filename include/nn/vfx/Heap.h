/**
 * @file Heap.h
 * @brief VFX heap implementation.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace vfx {
class Heap {
public:
    // The shared native destructor ACC3E0 is empty. Buffer::Initialize AD367C
    // and Finalize AD37B0 call these slots through nn::vfx::Heap parameters;
    // the complete SEAD adapter table 24CF050 confirms their order.
    virtual ~Heap() = default;
    virtual void* Alloc(size_t size, size_t alignment) = 0;
    virtual void Free(void* ptr) = 0;
};
}  // namespace vfx
}  // namespace nn
