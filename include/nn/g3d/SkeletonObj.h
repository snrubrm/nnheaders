#pragma once

#include "nn/g3d/ResSkeleton.h"
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/types.h>
#include <nn/util/MathTypes.h>

namespace nn::g3d {

class SkeletonObj {
public:
    const ResSkeleton* GetRes() const { return m_Res; }

    // Inline-only in the original; name is a guess. Initialize 0x7101337c1c copies the resource's
    // bone array (ResSkeletonData +0x18) to +0x10, and 0x7100f1a3b0 indexes it with the 0x50-byte
    // ResBone stride to read ResBone::parentIndex.
    const ResBone* GetBone(int index) const { return &m_Bones[index]; }

    // Inline-only in the original; names are guesses. ModelRenderCache::loadResource
    // 0x7100c809ac indexes the buffer array and passes the size to ShaderUtilG3D::load.
    gfx::TBuffer<gfx::ApiVariationNvn8>* GetBuffersMaybe() const { return mBuffers; }
    u64 GetBufferSizeMaybe() const { return mBufferSize; }

    /// 0x7101337d1c (declared only)
    void ClearLocalMtx();
    // 0x7101338974 / 0x71013389c0
    void CalculateWorldMtx(const nn::util::neon::MatrixRowMajor4x3fType& mtx);
    void CalculateSkeleton(int index);

private:
    const ResSkeleton* m_Res;
    u16 m_Flags;  // 8
    u8 _a[0x10 - 0xa];
    const ResBone* m_Bones;  // 10
    u8 _18[0x40 - 0x18];
    // ModelRenderCache::loadResource 0x7100c809ac indexes this array with the buffer index and
    // passes its +0x48 size to ShaderUtilG3D::load.
    gfx::TBuffer<gfx::ApiVariationNvn8>* mBuffers;  // 40
    u64 mBufferSize;  // 48
};

}  // namespace nn::g3d
