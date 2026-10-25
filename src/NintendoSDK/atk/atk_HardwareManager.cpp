#include <nn/atk/detail/driver/HardwareManager.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

int HardwareManager::GetChannelCountMax() const {
    return _758 ? 2 : 6;
}

}  // namespace driver
}  // namespace detail
}  // namespace atk
}  // namespace nn
