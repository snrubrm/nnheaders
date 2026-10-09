#include <nn/ui2d/DrawInfo.h>

#include <cstring>

namespace nn::ui2d {

// 0x7100ac06a4
// NON_MATCHING: aggregate identity initialization uses scalar stores instead of constant vectors.
DrawInfo::DrawInfo()
    : mProjMtx{{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}},
      mViewMtx{{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f}}},
      mModelViewMtx{{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f}}},
      mLocationAdjustScale{1.0f, 1.0f}, mGraphicsResource(nullptr), mLayout(nullptr),
      mUi2dConstantBuffer(nullptr), mFontConstantBuffer(nullptr), _d8{}, _eb{} {}

DrawInfo::~DrawInfo() = default;

// 0x7100ac0714
void DrawInfo::SetProjMtx(const util::Matrix4x4fType& matrix) {
    std::memcpy(&mProjMtx, &matrix, sizeof(mProjMtx));
}

}  // namespace nn::ui2d
