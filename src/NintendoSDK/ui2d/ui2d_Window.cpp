#include <nn/ui2d/Window.h>
#include <nn/ui2d/Material.h>

namespace nn::ui2d {

// The original preserves Window's vtable store before calling Pane's own
// destructor. Keep it as in GameDataFlagSelector (commit 96101229).
Window::~Window() {
    ;
}

util::Unorm8x4 Window::GetVertexColor(s32 index) const {
    return mVertexColors[index];
}

void Window::SetVertexColor(s32 index, const util::Unorm8x4& color) {
    mVertexColors[index] = color;
}

u8 Window::GetVertexColorElement(s32 index) const {
    return mVertexColors[index / 4].v[index % 4];
}

void Window::SetVertexColorElement(s32 index, u8 value) {
    mVertexColors[index / 4].v[index % 4] = value;
}

u8 Window::GetMaterialCount() const {
    return mFrameCount + 1;
}

// NON_MATCHING: final material-load merging and register allocation differ.
Material* Window::GetMaterial(s32 index) const {
    GetMaterialCount();  // Original virtual call's result is discarded, as in Picture.
    if (index == 0)
        return mContentMaterial;
    if (index <= mFrameCount)
        return mFrames[u32(index - 1)].mMaterial;
    return nullptr;
}

// 0x7100abd6b4
const Material* Window::FindMaterialByName(const char* name, bool recursive) const {
    return const_cast<Window*>(this)->FindMaterialByName(name, recursive);
}

// 0x7100abd5c0
Material* Window::FindMaterialByName(const char* name, bool recursive) {
    if (mContentMaterial && mContentMaterial->IsNameEqual(name))
        return mContentMaterial;
    for (s32 i = 0; i < mFrameCount; ++i) {
        Material* material = mFrames[i].mMaterial;
        if (material->IsNameEqual(name))
            return material;
    }
    if (recursive) {
        for (Pane& child : GetChildList()) {
            if (Material* material = child.FindMaterialByName(name, true))
                return material;
        }
    }
    return nullptr;
}

}  // namespace nn::ui2d
