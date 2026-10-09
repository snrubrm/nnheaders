#pragma once

#include <nn/types.h>

namespace nn::util::neon {

struct MatrixRowMajor4x3fType {
    // DrawInfo 0x7100ac06a4 and Matrix34 byte-copy consumers use three rows.
    f32 m[3][4];
};
static_assert(sizeof(MatrixRowMajor4x3fType) == 48);

struct MatrixColumnMajor4x3fType {
    f32 m[3][4];
};

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
