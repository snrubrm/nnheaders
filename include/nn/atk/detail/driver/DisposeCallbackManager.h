#pragma once

#include <nn/atk/detail/driver/DisposeCallback.h>

namespace nn::atk::detail::driver {

class DisposeCallbackManager {
public:
    static DisposeCallbackManager& GetInstance();
    void RegisterDisposeCallback(DisposeCallback* callback);
    void UnregisterDisposeCallback(DisposeCallback* callback);
    // Native callers discard the result; void is inferred.
    void Dispose(const void* address, size_t size);

private:
    using CallbackList = nn::util::IntrusiveList<
        DisposeCallback, nn::util::IntrusiveListMemberNodeTraits<DisposeCallback, &DisposeCallback::mNode>>;
    static_assert(sizeof(CallbackList) == sizeof(nn::util::detail::IntrusiveListImplementation));
    CallbackList mCallbacks;
};

}  // namespace nn::atk::detail::driver
