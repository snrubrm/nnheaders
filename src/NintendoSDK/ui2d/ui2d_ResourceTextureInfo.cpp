#include <nn/ui2d/ResourceTextureInfo.h>

namespace nn::ui2d {

// 0x7100be0ee0
void ResourceTextureInfo::Finalize(gfx::Device* device) {
    gfx::ResTexture* resource = mResource;
    if (resource) {
        gfx::Texture* texture = resource->GetTexture();
        if (texture && texture->IsInitialized()) {
            texture->Finalize(device);
            gfx::TextureView* view = resource->GetTextureView();
            view->Finalize(device);
            mResource = nullptr;
        }
    }
}

// 0x7100be0f38
TexSize ResourceTextureInfo::GetSize() const {
    const gfx::TextureInfo* info = mResource->GetTextureInfo();
    return TexSize(info->GetWidth(), info->GetHeight());
}

// 0x7100be0f4c
u32 ResourceTextureInfo::m5() const {
    return mResource->GetTextureInfo()->GetImageFormat();
}

// 0x7100be0f58
bool ResourceTextureInfo::IsValid() const {
    const TexSize size = GetSize();
    return size.width != 0 && size.height != 0;
}

// 0x7100be0f9c
gfx::TextureView* ResourceTextureInfo::GetTextureView() {
    return mResource->GetTextureView();
}

// 0x7100be0fa8
const gfx::TextureView* ResourceTextureInfo::GetTextureView() const {
    return mResource->GetTextureView();
}

}  // namespace nn::ui2d
