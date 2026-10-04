#pragma once

#include <nn/types.h>
#include <cstddef>

namespace nn::ui2d {

// The callback record constructed at 0xabff98 and consumed at 0xac2cb8.
// The callback supplies a shader name and three words whose roles are not yet
// established. The constructor leaves the alignment bytes at +6/+7 untouched.
struct UserShaderInformation {
    UserShaderInformation();

    char mName[6];
    u32 _8;
    u32 _c;
    u32 _10;
};
static_assert(sizeof(UserShaderInformation) == 0x14);
static_assert(offsetof(UserShaderInformation, _8) == 0x8);

}  // namespace nn::ui2d
