#include <nn/font/font_DispStringBuffer.h>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_BufferInfo.h>

namespace nn::font {

// 0x7101323fec
DispStringBuffer::DispStringBuffer() = default;

// 0x7101324014
DispStringBuffer::~DispStringBuffer() {}

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
    return header_size + characters_size + (arg._14 ? characters_size : 0);
}

}  // namespace nn::font
