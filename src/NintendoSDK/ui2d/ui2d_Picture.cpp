#include <nn/ui2d/Picture.h>

#include <nn/ui2d/Material.h>
#include <nn/ui2d/Layout.h>

namespace nn::ui2d {

// The original keeps the vtable store (a plain empty body drops it); `{ ; }` as in upstream's
// GameDataFlagSelector::~GameDataFlagSelector (commit 96101229).
// 0x7100ab9d94
Picture::~Picture() {
    ;
}

// 0x7100ab9ddc
void Picture::Finalize(gfx::Device* device) {
    Pane::Finalize(device);
    if (mMaterial && !mMaterial->IsUserAllocated()) {
        mMaterial->Finalize(device);
        Material* material = mMaterial;
        if (material) {
            material->~Material();
            Layout::FreeMemory(material);
        }
        mMaterial = nullptr;
    }
    mTexCoordArray.Free();
}

// 0x7100ab9e40
u8 Picture::GetMaterialCount() const {
    return mMaterial != nullptr;
}

// 0x7100ab9e50
Material* Picture::GetMaterial(s32 index) const {
    GetMaterialCount();  // result unused (in the target)
    if (index == 0)
        return mMaterial;
    return nullptr;
}

}  // namespace nn::ui2d
