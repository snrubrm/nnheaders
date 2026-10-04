/**
 * @file TexCoordArray.h
 * @brief Texture coordinate array implementation.
 */

#pragma once

#include <nn/types.h>

namespace nn {

namespace util {
struct Float2;
}

namespace ui2d {
class Layout;

namespace detail {
class TexCoordArray {
public:
    void Initialize();
    void Free();
    void Reserve(s32);
    void SetSize(s32 size);
    void GetCoord(nn::util::Float2*, s32) const;
    void SetCoord(s32, nn::util::Float2 const*);
    void Copy(void const*, s32);
    bool CompareCopiedInstanceTest(nn::ui2d::detail::TexCoordArray const&) const;

    // Reserve allocates 32 bytes per entry; coordinate accessors copy four Float2 values.
    using Quad = nn::util::Float2[4];

    u8 mCapacity;
    u8 mSize;
    u16 _2;
    u32 _4;                     // padding?
    Quad* mCoords;  // _8
};
}  // namespace detail
}  // namespace ui2d
}  // namespace nn
