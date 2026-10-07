#pragma once

namespace nn::atk::detail {

namespace Util {

/// Lazily constructed global instance (a function-local static in the sound library).
template <typename T>
class Singleton {
public:
    static T& GetInstance();
};

}  // namespace Util

}  // namespace nn::atk::detail
