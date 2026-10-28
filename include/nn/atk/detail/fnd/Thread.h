#pragma once

namespace nn::atk::detail::fnd {

// Partial: only the empty destructor and resume hook are modelled (0x710135d698 / 0x710135d898).
class Thread {
public:
    ~Thread();
    void Resume();
};

}  // namespace nn::atk::detail::fnd
