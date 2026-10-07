#pragma once

#include <nn/util/AccessorBase.h>
#include <nn/util/util_BinTypes.h>

namespace nn::g3d {
class ResAnimCurve;

// SDK Reset ac720c and binding 13333c8 independently establish the 0x28-byte
// bone-animation record, its name at +0 and curve array/count at +8/+1e.
struct ResBoneAnimData {
    nn::util::BinPtrToString pName;
    nn::util::BinTPtr<ResAnimCurve> pCurves;
    u8 _10[0xe];
    u8 curveCount;
    u8 _1f;
    u8 _20[8];
};

class ResBoneAnim : public nn::util::AccessorBase<ResBoneAnimData> {
public:
    // Inline-only in the original; name is a guess following ResBone's API.
    // SDK binding 13333c8 and AS lookup 115d55c both consume name data at +2.
    const char* GetName() const { return pName.Get()->GetData(); }
};
static_assert(sizeof(ResBoneAnim) == 0x28);
}  // namespace nn::g3d
