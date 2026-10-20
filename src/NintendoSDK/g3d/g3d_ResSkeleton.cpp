#include <nn/g3d/ResSkeleton.h>

namespace nn::g3d {

void ResSkeleton::Reset(nn::Bit32 flag) {
    if (!(flag & ResetGuardFlag_UserPtr))
        SetUserPtr(nullptr);
}

}  // namespace nn::g3d
