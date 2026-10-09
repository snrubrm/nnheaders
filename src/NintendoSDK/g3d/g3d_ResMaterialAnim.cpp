#include <nn/g3d/ResMaterialAnim.h>

namespace nn::g3d {

static_assert(sizeof(ResMaterialAnim) == 0x78);

// NON_MATCHING: Natural validity branches select status flags and allocate registers differently.
s32 ResMaterialAnim::BindTexture(TextureRef (*callback)(const char*, void*), void* userData) {
    s32 result = 0;
    const int count = mTextureCount;
    for (int i = 0; i < count; ++i) {
        if (mTextureViews[i] && mTextureDescriptorSlots[i].IsValid())
            continue;
        const TextureRef ref = callback(mTextureNames[i].Get()->GetData(), userData);
        mTextureViews[i] = ref.pTextureView;
        mTextureDescriptorSlots[i] = ref.descriptorSlot;
        if (!ref.pTextureView || !ref.descriptorSlot.IsValid())
            result |= 0x10000;
        else
            result |= 1;
    }
    return result;
}

void ResMaterialAnim::ReleaseTexture() {
    const int count = mTextureCount;
    for (int i = 0; i < count; ++i) {
        mTextureViews[i] = nullptr;
        mTextureDescriptorSlots[i].Invalidate();
    }
}

}  // namespace nn::g3d
