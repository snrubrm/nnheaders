#include <nn/font/font_DispStringBuffer.h>

namespace nn::font {

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

}  // namespace nn::font
