#pragma once

#include <nn/types.h>
#include <math/seadMathBase.h>

namespace nn::util::neon {

// The native DrawInfo constructor 0x7100ac06a4 initialises the view matrix at +0x50 with the same
// three 16-byte identity rows as a sead::BaseMtx34<float>, SetupDrawInfoPerspective 0x7100bee700
// copies a sead::Matrix34f's rows into it, and Unk_71024f3a60::doInitialize 0x7100f0a7f0 passes
// &DrawInfo::mViewMtx straight to sead::Matrix34CalcCommon<float>::inverse, which takes a
// sead::BaseMtx34<float>&. The SDK's row-major 4x3 matrix is therefore sead's 3x4 matrix; deriving
// from it keeps the nn::util::neon::MatrixRowMajor4x3fType mangled names and the 3x4 float layout
// while allowing the implicit reference conversion the original source relies on.
struct MatrixRowMajor4x3fType : sead::BaseMtx34<f32> {
    // DrawInfo 0x7100ac06a4 and Matrix34 byte-copy consumers use three rows.
};
static_assert(sizeof(MatrixRowMajor4x3fType) == 48);

// The transposed 4x3 form the SDK returns from Pane::GetMtx is a 3x4 float matrix in memory:
// eui::EffectLinkUser::m1 0x71009d00b8 loads three 16-byte rows from the pane's GetMtx result and
// transposes them with the sead 4x4 routine, so the type has to convert to sead::BaseMtx34<float>
// the same way MatrixRowMajor4x3fType does.
struct MatrixColumnMajor4x3fType : sead::BaseMtx34<f32> {
};
static_assert(sizeof(MatrixColumnMajor4x3fType) == 48);

struct MatrixRowMajor4x4fType {
    f32 m[4][4];
};

struct MatrixColumnMajor4x4fType {
    f32 m[4][4];
};

}  // namespace nn::util::neon

namespace nn::util {

typedef uint32_t AngleIndex;

struct Float2 {
    union {
        f32 v[2];
        struct {
            f32 x;
            f32 y;
        };
    };
};

struct Float3 {
    union {
        f32 v[3];
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
    };
};

struct Float4 {
    union {
        f32 v[4];
        struct {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };
    };
};

struct FloatColumnMajor4x3 {
    f32 m[3][4];
};

struct Unorm8x4 {
    union {
        uint8_t v[4];
    };
};

typedef Unorm8x4 Color4u8Type;

typedef neon::MatrixRowMajor4x3fType Matrix4x3fType;
typedef neon::MatrixRowMajor4x4fType Matrix4x4fType;
typedef neon::MatrixColumnMajor4x3fType MatrixT4x3fType;
typedef neon::MatrixColumnMajor4x4fType MatrixT4x4fType;

}  // namespace nn::util
