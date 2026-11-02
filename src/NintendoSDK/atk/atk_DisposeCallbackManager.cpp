#include <nn/atk/detail/driver/DisposeCallbackManager.h>

namespace nn::atk::detail::driver {

DisposeCallbackManager& DisposeCallbackManager::GetInstance() {
    static DisposeCallbackManager instance;
    return instance;
}

void DisposeCallbackManager::RegisterDisposeCallback(DisposeCallback* callback) {
    mCallbacks.push_back(*callback);
}

void DisposeCallbackManager::UnregisterDisposeCallback(DisposeCallback* callback) {
    mCallbacks.erase(mCallbacks.iterator_to(*callback));
}

// NON_MATCHING: the compiler inlines the singleton guard at each native call site.
void DisposeCallbackManager::Dispose(const void* address, size_t size) {
    auto& callbacks = GetInstance().mCallbacks;
    auto it = callbacks.begin();
    GetInstance();
    if (it != callbacks.end()) {
        const auto* end = static_cast<const u8*>(address) + size;
        do {
            DisposeCallback& callback = *it++;
            callback.InvalidateData(address, end);
            GetInstance();
        } while (it != callbacks.end());
    }
}

}  // namespace nn::atk::detail::driver
