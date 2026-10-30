#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {

class DisposeCallback {
public:
    virtual ~DisposeCallback();
    // The slot is established by native dispatch; whether the base is pure is unresolved.
    virtual void InvalidateData(const void* begin, const void* end);

private:
    friend class DisposeCallbackManager;
    nn::util::IntrusiveListNode mNode;  // +0x8
};
static_assert(sizeof(DisposeCallback) == 0x18);

}  // namespace nn::atk::detail::driver
