#include <nn/atk/detail/CurveLfoParam.h>
#include <nn/atk/detail/CurveLfo.h>
#include <nn/atk/detail/Util.h>
#include <nn/util/util_Arithmetic.h>

namespace nn::atk::detail {

namespace {
using CurveFunction = f32 (*)(f32);
// InitializeCurveTable and GetValue directly address this original 128-entry table.
CurveFunction sCurveTable[128];

f32 sub_7101353C08(f32 phase) {
    return nn::util::SinTable(static_cast<nn::util::AngleIndex>(phase * 4294967296.0f));
}
// Update normalizes phase to [0, 1), proving only these four used entries.
// The neighboring unused float at each native table boundary remains unmodeled.
const f32 sUnk_7101ECB5E8[4] = {4.0f, -4.0f, -4.0f, 4.0f};
const f32 sUnk_7101ECB5FC[4] = {0.0f, 2.0f, 2.0f, -4.0f};

f32 sub_7101353C44(f32 phase) {
    const u32 index = u32(phase * 4.0f);
    return sUnk_7101ECB5E8[index] * phase + sUnk_7101ECB5FC[index];
}
f32 sub_7101353C8C(f32) {
    return static_cast<f32>(Util::sub_71013548CC() & 0xffff) / 65535.0f;
}

f32 sub_7101353C70(f32 phase) {
    return phase;
}

f32 sub_7101353C74(f32 phase) {
    return phase < 0.5f ? 0.0f : 1.0f;
}
}  // namespace

void CurveLfo::InitializeCurveTable() {
    for (auto& curve : sCurveTable)
        curve = nullptr;
    sCurveTable[0] = sub_7101353C08;
    sCurveTable[1] = sub_7101353C44;
    sCurveTable[2] = sub_7101353C70;
    sCurveTable[3] = sub_7101353C74;
    sCurveTable[4] = sub_7101353C8C;
}

void CurveLfoParam::Initialize() {
    mDepth = 0.0f;
    mSpeed = 6.25f;
    mDelay = 0;
    mRange = 1;
    mCurve = 0;
    mPhase = 0;
}

}  // namespace nn::atk::detail
