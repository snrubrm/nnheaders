#include <nn/g3d/ModelObj.h>
#include <nn/g3d/SkeletonObj.h>

namespace nn::g3d {

void ModelObj::CalculateWorld(const nn::util::neon::MatrixRowMajor4x3fType& mtx) {
    m_Skeleton->CalculateWorldMtx(mtx);
}

void ModelObj::CalculateSkeleton(int index) {
    m_Skeleton->CalculateSkeleton(index);
}

}  // namespace nn::g3d
