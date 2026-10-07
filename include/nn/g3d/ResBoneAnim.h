#pragma once

#include <nn/util/AccessorBase.h>
#include <nn/util/util_BinTypes.h>
#include <nn/util/MathTypes.h>

namespace nn::g3d {
class ResAnimCurve;
class ResBone;

// ClearResult 1333e5c advances each result by 0x30; ac6f68 writes these
// transform components and leaves the original interval at +1c untouched.
struct BoneAnimResult {
    u32 flag;
    nn::util::Float3 scale;
    nn::util::Float3 translate;
    u32 _1c;
    nn::util::Float4 rotate;
};
static_assert(sizeof(BoneAnimResult) == 0x30);

// SDK Reset ac720c and binding 13333c8 independently establish the 0x28-byte
// bone-animation record, its name at +0 and curve array/count at +8/+1e.
struct ResBoneAnimData {
    nn::util::BinPtrToString pName;
    nn::util::BinTPtr<ResAnimCurve> pCurves;
    // Result initialization ac6f68 consumes the sequential optional base values
    // at +10 and their presence/transform flags at +18.
    nn::util::BinTPtr<float> pBaseValues;
    u32 flag;
    u16 _1c;
    u8 curveCount;
    u8 _1f;
    u8 _20[8];
};

class ResBoneAnim : public nn::util::AccessorBase<ResBoneAnimData> {
public:
    void sub_7100AC6F68(BoneAnimResult* result, const ResBone* bone) const;

    // Inline-only in the original; name is a guess following ResBone's API.
    // SDK binding 13333c8 and AS lookup 115d55c both consume name data at +2.
    const char* GetName() const { return pName.Get()->GetData(); }
};
static_assert(sizeof(ResBoneAnim) == 0x28);
}  // namespace nn::g3d
