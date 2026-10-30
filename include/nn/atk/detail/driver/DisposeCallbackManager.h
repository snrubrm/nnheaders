#pragma once

#include <nn/atk/detail/driver/DisposeCallback.h>

namespace nn::atk::detail::driver {

class DisposeCallbackManager {
public:
    static DisposeCallbackManager& GetInstance();
    void RegisterDisposeCallback(DisposeCallback* callback);
    void UnregisterDisposeCallback(DisposeCallback* callback);

private:
    nn::util::detail::IntrusiveListImplementation mCallbacks;
};

}  // namespace nn::atk::detail::driver
