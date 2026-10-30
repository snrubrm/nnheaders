#include <nn/atk/detail/driver/DisposeCallbackManager.h>

namespace nn::atk::detail::driver {

DisposeCallbackManager& DisposeCallbackManager::GetInstance() {
    static DisposeCallbackManager instance;
    return instance;
}

void DisposeCallbackManager::RegisterDisposeCallback(DisposeCallback* callback) {
    mCallbacks.push_back(callback->mNode);
}

void DisposeCallbackManager::UnregisterDisposeCallback(DisposeCallback* callback) {
    mCallbacks.erase(mCallbacks.iterator_to(callback->mNode));
}

}  // namespace nn::atk::detail::driver
