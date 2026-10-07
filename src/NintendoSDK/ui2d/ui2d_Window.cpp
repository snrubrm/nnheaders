#include <nn/ui2d/Window.h>

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

}  // namespace nn::ui2d
