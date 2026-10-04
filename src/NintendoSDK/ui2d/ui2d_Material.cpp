#include <nn/ui2d/Material.h>

namespace nn::ui2d {

// 0x7100aba544
TexMap::TexMap() : mTextureInfo(nullptr) {
    mFlags = 0x90;
}

// 0x7100aba554
TexMap::TexMap(const TextureInfo* info) {
    Set(info);
}

// 0x7100aba57c
TexMap::~TexMap() {}

// 0x7100aba580
void TexMap::Finalize() {}

// 0x7100aba568
void TexMap::Set(const TextureInfo* info) {
    if (info) {
        mTextureInfo = info;
        mFlags = 0x90;
    }
}

// 0x7100aba584
void TexMap::SetWrapMode(TexWrap wrap_s, TexWrap wrap_t) {
    mWrapS = wrap_s;
    mWrapT = wrap_t;
}

// 0x7100aba5a0
void TexMap::SetFilter(TexFilter min_filter, TexFilter mag_filter) {
    mMinFilter = min_filter;
    mMagFilter = mag_filter;
}

}  // namespace nn::ui2d
