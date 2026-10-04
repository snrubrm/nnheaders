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

// 0x7100ab9fcc
util::Unorm8x4 Picture::GetVertexColor(s32 index) const {
    return mVertexColors[index];
}

// 0x7100ab9fd8
void Picture::SetVertexColor(s32 index, const util::Unorm8x4& color) {
    mVertexColors[index] = color;
}

// 0x7100ab9fe8
u8 Picture::GetVertexColorElement(s32 index) const {
    return mVertexColors[index / 4].v[index % 4];
}

// 0x7100aba010
void Picture::SetVertexColorElement(s32 index, u8 value) {
    mVertexColors[index / 4].v[index % 4] = value;
}

// 0x7100ab9fbc
void Picture::sub_7100AB9FBC(util::Float2* coordinates, s32 index) const {
    mTexCoordArray.GetCoord(coordinates, index);
}

// 0x7100ab9fc4
void Picture::sub_7100AB9FC4(s32 index, const util::Float2* coordinates) {
    mTexCoordArray.SetCoord(index, coordinates);
}

}  // namespace nn::ui2d
