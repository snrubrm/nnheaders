#include <nn/ui2d/Material.h>
#include <nn/ui2d/AnimTransform.h>
#include <nn/ui2d/BuildTypes.h>
#include <nn/util/util_BytePtr.h>

namespace nn::ui2d {

namespace detail {
// 0x7100ac01fc
const ResMaterial* GetResMaterial(const BuildResSet* resources, u16 index) {
    const u32* offsets = util::ConstBytePtr(resources->mMaterialList, 0xc).Get<u32>();
    return util::ConstBytePtr(resources->mMaterialList, offsets[index]).Get<ResMaterial>();
}
}  // namespace detail

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

// 0x7100ac3720
void Material::BindAnimation(AnimTransform* transform) {
    transform->BindMaterial(this);
}

// 0x7100ac3738
void Material::UnbindAnimation(AnimTransform* transform) {
    transform->UnbindMaterial(this);
}

}  // namespace nn::ui2d
