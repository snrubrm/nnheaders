#include <nn/ui2d/detail/TexCoordArray.h>

#include <nn/ui2d/Layout.h>
#include <nn/util/MathTypes.h>

namespace nn::ui2d::detail {

// 0x7100abffac
void TexCoordArray::Initialize() {
    mCapacity = 0;
    mSize = 0;
    mCoords = nullptr;
}

// 0x7100abffb8
void TexCoordArray::Free() {
    if (mCoords) {
        Layout::FreeMemory(mCoords);
        mCoords = nullptr;
        mCapacity = 0;
        mSize = 0;
    }
}

// 0x7100ac0048
// NON_MATCHING: loop peeling and constant coordinate stores differ.
void TexCoordArray::SetSize(s32 size) {
    if (!mCoords || mCapacity < size)
        return;
    for (s32 i = mSize; i < size; ++i) {
        mCoords[i][0] = {{0.0f, 0.0f}};
        mCoords[i][1] = {{1.0f, 0.0f}};
        mCoords[i][2] = {{0.0f, 1.0f}};
        mCoords[i][3] = {{1.0f, 1.0f}};
    }
    mSize = size;
}

// 0x7100ac0100
void TexCoordArray::GetCoord(nn::util::Float2* coords, s32 index) const {
    for (s32 i = 0; i < 4; ++i)
        coords[i] = mCoords[index][i];
}

// 0x7100ac0144
void TexCoordArray::SetCoord(s32 index, const nn::util::Float2* coords) {
    for (s32 i = 0; i < 4; ++i)
        mCoords[index][i] = coords[i];
}

// 0x7100ac0188
// NON_MATCHING: size selection and Float2-copy scheduling differ.
void TexCoordArray::Copy(const void* data, s32 size) {
    if (mSize < static_cast<u8>(size))
        mSize = size;
    const auto* coords = static_cast<const Quad*>(data);
    for (s32 i = 0; i < size; ++i) {
        for (s32 j = 0; j < 4; ++j)
            mCoords[i][j] = coords[i][j];
    }
}

}  // namespace nn::ui2d::detail
