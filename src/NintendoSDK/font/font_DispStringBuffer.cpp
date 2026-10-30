#include <nn/font/font_DispStringBuffer.h>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_BufferInfo.h>

namespace nn::font {

// 0x7101323fec
DispStringBuffer::DispStringBuffer() = default;

// 0x7101324014
DispStringBuffer::~DispStringBuffer() {}

void DispStringBuffer::SetGpuBuffer(GpuBuffer* buffer) {
    _b0 = buffer;
}

void DispStringBuffer::SetFontHeight(f32 height) {
    mFontHeight = height;
}

// 0x7101324018
bool DispStringBuffer::Initialize(gfx::Device*, const InitializeArg& arg) {
    if (mCapacity > 0)
        return false;
    if (arg.mCapacity < 1)
        return false;
    UnkA0* buffer = arg._0;
    if (!buffer)
        return false;
    mCapacity = arg.mCapacity;
    mCharCount = 0;
    _a0 = buffer;
    _a8 = reinterpret_cast<u8*>(buffer + arg.mCapacity);
    _b8 = arg._14;
    _b9 = arg._15;
    _ba = arg._16;
    _b0 = arg._8;
    return true;
}

// 0x7101324090
void DispStringBuffer::sub_7101324090(gfx::Device*) {
    if (mCapacity >= 1)
        mCapacity = 0;
}

// 0x71013240a4
size_t DispStringBuffer::sub_71013240A4(const InitializeArg& arg) {
    return arg.mCapacity * (sizeof(UnkA0) + sizeof(u8));
}

// 0x71013240b4
// NON_MATCHING: BufferInfo zero initialization and arithmetic/register scheduling differ.
size_t DispStringBuffer::sub_71013240B4(gfx::Device* device, const InitializeArg& arg) {
    gfx::BufferInfo info;
    info.SetDefault();
    info.SetGpuAccessFlags(gfx::GpuAccess_ConstantBuffer);
    const size_t header_alignment = gfx::TBuffer<gfx::ApiVariationNvn8>::GetBufferAlignment(device, info);
    const size_t header_size = ((128 + header_alignment - 1) & -header_alignment) << arg._15;
    const size_t character_size = arg._16 ? 96 : 64;
    const s32 capacity = arg.mCapacity;
    const size_t character_alignment = gfx::TBuffer<gfx::ApiVariationNvn8>::GetBufferAlignment(device, info);
    const size_t characters_size =
        (character_size * capacity + character_alignment - 1) & -character_alignment;
    return header_size + characters_size + (arg._14 == 0 ? 0 : characters_size);
}

// NON_MATCHING: scalar matrix loops and compiler-generated copies differ from native SIMD.
// 0x7101324354
void DispStringBuffer::BuildCommonConstantBufferData(
    ShaderParam& param, const util::MatrixT4x4fType& projection,
    const ConstantBufferAdditionalContent& content) const {
    if (mCapacity < 1)
        return;

    util::MatrixT4x4fType matrix = projection;
    if (content._18) {
        util::MatrixT4x4fType transformed;
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                transformed.m[row][column] =
                    content._18->m[0][column] * matrix.m[row][0] +
                    content._18->m[1][column] * matrix.m[row][1] +
                    content._18->m[2][column] * matrix.m[row][2] +
                    (column == 3 ? 1.0f : 0.0f) * matrix.m[row][3];
            }
        }
        matrix = transformed;
    }
    if (content._20) {
        util::MatrixT4x4fType transformed;
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                transformed.m[row][column] =
                    content._20->m[0][column] * matrix.m[row][0] +
                    content._20->m[1][column] * matrix.m[row][1] +
                    content._20->m[2][column] * matrix.m[row][2] +
                    (column == 3 ? 1.0f : 0.0f) * matrix.m[row][3];
            }
        }
        matrix = transformed;
    }
    param.matrix = matrix;
    param._40 = {{content._38.x - content._28.x,
                  content._38.y - content._28.y,
                  content._38.z - content._28.z,
                  content._38.w * (static_cast<f32>(content._48) * (1.0f / 255.0f))}};
    param._50 = {{content._28.x, content._28.y, content._28.z, 0.0f}};
    param._60 = {{content._5c.x - content._4c.x,
                  content._5c.y - content._4c.y,
                  content._5c.z - content._4c.z,
                  content._5c.w * (static_cast<f32>(content._6c) * (1.0f / 255.0f))}};
    param._70 = {{content._4c.x, content._4c.y, content._4c.z, 0.0f}};
}

}  // namespace nn::font
