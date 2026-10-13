#include <nn/atk/detail/CurveLfoParam.h>

namespace nn::atk::detail {

void CurveLfoParam::Initialize() {
    mDepth = 0.0f;
    mSpeed = 6.25f;
    mDelay = 0;
    mRange = 1;
    mCurve = 0;
    mPhase = 0;
}

}  // namespace nn::atk::detail
