#include <nn/font/font_TextureObject.h>

namespace nn::font {

const gfx::TextureView* PlacementTextureObject::GetTextureView() const {
    return nullptr;
}

gfx::TextureView* PlacementTextureObject::GetTextureView() {
    return nullptr;
}

// 0x7100ab3540
void TextureObject::Set(const void* data, u16 format, u16 width, u16 height, u8 sheet_count,
                        bool black_white_interpolation) {
    mBlackWhiteInterpolation = black_white_interpolation;
    mData = data;
    mFormat = format;
    mWidth = width;
    mHeight = height;
    mSheetCount = sheet_count;
    _18 = true;
}

}  // namespace nn::font
