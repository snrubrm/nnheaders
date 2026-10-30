#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {

class DisposeCallback {
public:
    virtual ~DisposeCallback();
    virtual void InvalidateData(const void* begin, const void* end) = 0;

private:
    nn::util::IntrusiveListNode mNode;  // +0x8
};
static_assert(sizeof(DisposeCallback) == 0x18);

}  // namespace nn::atk::detail::driver
