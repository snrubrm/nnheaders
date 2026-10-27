#include <nn/g3d/ResMaterial.h>

namespace nn::g3d {

size_t ResShaderParam::GetSize(Type type) {
    const s32 value = s32(type);
    if (value <= 0xf)
        return ((value & 3) << 2) + 4;
    if (value <= 0x1b)
        return size_t(((value - 0x10) >> 2) + 2) << 4;
    static const u64 sSizes[] = {0x18, 0x30, 0x18};
    return sSizes[value - 0x1c];
}

static_assert(sizeof(ResMaterial) == 0xb8);

// NON_MATCHING: Natural validity branches select status flags and allocate registers differently.
u64 ResMaterial::BindTexture(TextureRef (*callback)(const char*, void*), void* userData) {
    u64 result = 0;
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

void ResMaterial::ReleaseTexture() {
    const int count = mTextureCount;
    for (int i = 0; i < count; ++i) {
        mTextureViews[i] = nullptr;
        mTextureDescriptorSlots[i].Invalidate();
    }
}

}  // namespace nn::g3d
