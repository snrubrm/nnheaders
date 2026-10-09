#include <nn/g3d/ResFile.h>
#include <nn/g3d/ResModel.h>
#include <nn/g3d/ResMaterialAnim.h>

namespace nn::g3d {

// NON_MATCHING: Existing BindTexture return declarations produce different status-width arithmetic.
s32 ResFile::BindTexture(TextureRef (*callback)(const char*, void*), void* userData) {
    s32 result = 0;
    const int modelCount = mModelCount;
    for (int i = 0; i < modelCount; ++i)
        result |= mModels[i].BindTexture(callback, userData);
    const int animCount = mMatAnimCount;
    for (int i = 0; i < animCount; ++i)
        result |= mMatAnims[i].BindTexture(callback, userData);
    return result;
}

void ResFile::ReleaseTexture() {
    const int modelCount = mModelCount;
    for (int i = 0; i < modelCount; ++i)
        mModels[i].ReleaseTexture();
    const int animCount = mMatAnimCount;
    for (int i = 0; i < animCount; ++i)
        mMatAnims[i].ReleaseTexture();
}

}  // namespace nn::g3d
