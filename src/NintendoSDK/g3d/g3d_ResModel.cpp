#include <nn/g3d/ResModel.h>
#include <nn/g3d/ResMaterial.h>

namespace nn::g3d {

static_assert(sizeof(ResModel) == 0x78);

// NON_MATCHING: Status accumulation and register allocation differ with the existing u64 return API.
u64 ResModel::BindTexture(TextureRef (*callback)(const char*, void*), void* userData) {
    u64 result = 0;
    const int count = mMaterialCount;
    for (int i = 0; i < count; ++i)
        result |= mMaterials[i].BindTexture(callback, userData);
    return result;
}

void ResModel::ReleaseTexture() {
    const int count = mMaterialCount;
    for (int i = 0; i < count; ++i)
        mMaterials[i].ReleaseTexture();
}

}  // namespace nn::g3d
