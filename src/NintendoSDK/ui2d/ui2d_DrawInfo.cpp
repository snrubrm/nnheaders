#include <nn/ui2d/DrawInfo.h>

#include <cstring>

namespace nn::ui2d {

DrawInfo::~DrawInfo() = default;

// 0x7100ac0714
void DrawInfo::SetProjMtx(const util::Matrix4x4fType& matrix) {
    std::memcpy(&mProjMtx, &matrix, sizeof(mProjMtx));
}

}  // namespace nn::ui2d
