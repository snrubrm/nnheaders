/**
 * @file SoundInstanceManager.h
 * @brief Manager of the sound instances of one runtime.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace atk {
namespace detail {

// Partial model: the instance list starts at +0x20 (SetupUserParam of the runtimes walks it from
// there) and the manager is 0x30 bytes; the node type and the other members are not recovered.
template <typename T>
class SoundInstanceManager {
public:
    // 0x710133e770 / 0x7101349b74 / 0x710135b32c: sorts the instance list by priority.
    void SortPriorityList();

private:
    u8 _0[0x30];
};

}  // namespace detail
}  // namespace atk
}  // namespace nn
