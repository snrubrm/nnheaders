#pragma once

#include "nn/g3d/ResSkeleton.h"
#include <nn/util/MathTypes.h>

namespace nn::g3d {

// TODO
class SkeletonObj {
public:
    const ResSkeleton* GetRes() const { return m_Res; }
    /// 0x7101337d1c (declared only)
    void ClearLocalMtx();
    // 0x7101338974 / 0x71013389c0
    void CalculateWorldMtx(const nn::util::neon::MatrixRowMajor4x3fType& mtx);
    void CalculateSkeleton(int index);

private:
    const ResSkeleton* m_Res;
    // TODO: the rest of the members
};

}  // namespace nn::g3d
