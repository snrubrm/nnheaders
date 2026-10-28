#pragma once

namespace nn::atk::detail::fnd {

// Partial: only the two empty heap lock hooks are modelled (0x710135c258 / 0x710135c25c).
class HeapBase {
public:
    void LockHeap();
    void UnlockHeap();
};

}  // namespace nn::atk::detail::fnd
