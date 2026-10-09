#include <nn/ui2d/DrawInfo.h>
#include <nn/ui2d/GraphicsResource.h>

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

// 0x7100ac0720
void DrawInfo::sub_7100AC0720(gfx::CommandBuffer* command_buffer) {
    if (_eb[4]) {
        _eb[4] = 0;
        mGraphicsResource->sub_7100AC1244(command_buffer);
    }
}

// 0x7100ac0738
// NON_MATCHING: aggregate matrix assignment calls memcpy instead of emitting native vector copies.
void DrawInfo::sub_7100AC0738(util::Matrix4x4fType* matrix) const {
    *matrix = mProjMtx;
}

// 0x7100ac074c
// NON_MATCHING: aggregate matrix assignment calls memcpy instead of emitting native vector copies.
void DrawInfo::sub_7100AC074C(util::Matrix4x3fType* matrix) {
    if (!_eb[3]) {
        _eb[3] = 1;
        *matrix = mModelViewMtx;
    }
}

}  // namespace nn::ui2d
