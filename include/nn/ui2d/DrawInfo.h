/**
 * @file DrawInfo.h
 * @brief Per-draw state of the layout (matrices, alpha) passed to Pane::Calculate / Draw.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/types.h>
#include <nn/util/MathTypes.h>

namespace nn {
namespace font {
class GpuBuffer;
}
}  // namespace nn

namespace nn::ui2d {

class GraphicsResource;
class Layout;

// Layout evidence: DrawInfo::DrawInfo (0x7100ac06a4) stores identity matrices at +0x10 (4x4), +0x50 and +0x80
// (4x3 each) and 1.0 / 1.0 at +0xb0; the next function (0x7100ac0714) copies a 4x4 matrix to +0x10; Pane::LoadMtx
// copies the pane matrix to +0x80; eui::SetupDrawInfoOrtho writes the camera matrix to +0x50 and projects through
// the +0x10 setter. Member names follow the NintendoWare layout library's DrawInfo; the rest is unnamed.
// Virtual slots (vtable 0x249e6f8): GetRuntimeTypeInfo, destructor (D1 / D0). eui::DrawInfoEx (0x120 bytes)
// derives from it and starts at +0xf8.
class DrawInfo {
public:
    NN_RUNTIME_TYPEINFO_BASE()

    DrawInfo();
    virtual ~DrawInfo();

    // 0x7100ac0714 (name after the NintendoWare layout library; copies the matrix to +0x10)
    void SetProjMtx(const util::Matrix4x4fType& matrix);

    /* 0x08 */ u64 _8;
    /* 0x10 */ util::Matrix4x4fType mProjMtx;
    /* 0x50 */ util::Matrix4x3fType mViewMtx;
    /* 0x80 */ util::Matrix4x3fType mModelViewMtx;
    /* 0xb0 */ util::Float2 mLocationAdjustScale;
    /* 0xb8 */ GraphicsResource* mGraphicsResource;
    /* 0xc0 */ const Layout* mLayout;
    // The two GPU buffers the layout draws from; eui::ConstantBuffer (0x7100bf4368) sets them, and
    // Picture::DrawSelf (0x7100aba44c) reads a flag byte of the first. Names follow the NintendoWare layout
    // library; which one is which is a guess.
    /* 0xc8 */ font::GpuBuffer* mUi2dConstantBuffer;
    /* 0xd0 */ font::GpuBuffer* mFontConstantBuffer;
    /* 0xd8 */ u64 _d8[2];  // zero-initialised
    /* 0xe8 */ u8 _e8[3];
    /* 0xeb */ u8 _eb[7];  // zero-initialised flag bytes (Pane::LoadMtx clears the one at +0xee)
};
static_assert(sizeof(DrawInfo) == 0xf8);

}  // namespace nn::ui2d
