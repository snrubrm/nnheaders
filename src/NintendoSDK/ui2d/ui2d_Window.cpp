#include <nn/ui2d/Window.h>
#include <nn/ui2d/Material.h>
#include <nn/ui2d/Layout.h>
#include <nn/ui2d/ResPane.h>
#include <new>

namespace nn::ui2d {

// 0x7100abd120
// NON_MATCHING: native copies margin groups as whole values and schedules sizing branches differently.
Window::Window(const Window& source, gfx::Device* device) : Pane(source) {
    for (s32 i = 0; i < 4; ++i) {
        mContentMargins[i] = source.mContentMargins[i];
        mFrameMargins[i] = source.mFrameMargins[i];
    }
    mTexCoordArray.Initialize();
    mWindowKind = source.mWindowKind;
    mFrameCount = source.mFrameCount;
    mWindowFlags = source.mWindowFlags;
    mFrameConstantBufferCount = 0;
    for (s32 i = 0; i < 4; ++i)
        mVertexColors[i] = source.mVertexColors[i];

    const u8 coordinate_count = source.mTexCoordArray.mSize;
    if (coordinate_count) {
        mTexCoordArray.Reserve(coordinate_count);
        mTexCoordArray.SetSize(coordinate_count);
        for (s32 i = 0; i < coordinate_count; ++i)
            mTexCoordArray.SetCoord(i, source.mTexCoordArray.mCoords[i]);
    }

    // Allocators can call user code, so preserve each source material across the call.
    const Material* content_material = source.mContentMaterial;
    void* memory = Layout::AllocateMemory(sizeof(Material), 4);
    mContentMaterial = memory ? new (memory) Material(*content_material, device) : nullptr;

    const s8 frame_count = mFrameCount;
    mFrameCount = 0;
    void* frame_memory = Layout::AllocateMemory(sizeof(Frame) * frame_count, 4);
    if (frame_memory) {
        Frame* frames = static_cast<Frame*>(frame_memory);
        for (s32 i = 0; i < frame_count; ++i) {
            new (&frames[i]) Frame;
            frames[i].mTextureFlip = 0;
            frames[i].mMaterial = nullptr;
        }
        mFrames = frames;
        mFrameCount = frame_count;
    } else {
        mFrames = nullptr;
    }
    for (s32 i = 0; i < mFrameCount; ++i) {
        mFrames[i].mTextureFlip = source.mFrames[i].mTextureFlip;
        const Material* frame_material = source.mFrames[i].mMaterial;
        memory = Layout::AllocateMemory(sizeof(Material), 4);
        mFrames[i].mMaterial = memory ? new (memory) Material(*frame_material, device) : nullptr;
    }

    if (source.mFrameConstantBufferOffsets) {
        const u32 offset_count = mWindowKind == 1 || mWindowKind == 2 ? 1 : 3;
        mFrameConstantBufferOffsets = static_cast<u32*>(
            Layout::AllocateMemory(sizeof(u32) * offset_count, 4));
        mFrameConstantBufferCount = offset_count;
    } else {
        mFrameConstantBufferOffsets = nullptr;
    }
}

// The original preserves Window's vtable store before calling Pane's own
// destructor. Keep it as in GameDataFlagSelector (commit 96101229).
Window::~Window() {
    ;
}

// 0x7100abd038
// NON_MATCHING: native orders the mode branches differently and retains a width self-store.
void Window::InitializeSize(const ResWindow* resource) {
    for (s32 i = 0; i < 4; ++i)
        mFrameMargins[i] = resource->mFrameMargins[i];
    if (mFrameMargins[0] || mFrameMargins[1] || mFrameMargins[2] || mFrameMargins[3])
        mWindowFlags |= 8;

    switch (mWindowKind) {
    case 1:
        mContentMargins[0] = resource->mContentMargins[0];
        mContentMargins[1] = resource->mContentMargins[1];
        mContentMargins[2] = 0;
        mContentMargins[3] = 0;
        SetSize({GetSize().width, static_cast<f32>(resource->mContentMargins[2])});
        break;
    case 2:
        mContentMargins[0] = static_cast<s32>(GetSize().width) - resource->mContentMargins[1];
        mContentMargins[1] = resource->mContentMargins[1];
        mContentMargins[2] = 0;
        mContentMargins[3] = 0;
        SetSize({GetSize().width, static_cast<f32>(resource->mContentMargins[2])});
        break;
    default:
        for (s32 i = 0; i < 4; ++i)
            mContentMargins[i] = resource->mContentMargins[i];
        break;
    }
}

// 0x7100abd3cc
// NON_MATCHING: native merges the first loop exit into the destruction pass differently.
void Window::Finalize(gfx::Device* device) {
    Pane::Finalize(device);
    if (mFrameConstantBufferOffsets)
        Layout::FreeMemory(mFrameConstantBufferOffsets);

    for (s32 i = 0; i < mFrameCount; ++i)
        mFrames[i].mMaterial->Finalize(device);

    // Destruction may invoke user code; keep the native frame array and count for this pass.
    Frame* frames = mFrames;
    const s32 count = mFrameCount;
    if (frames) {
        for (s32 i = 0; i < count; ++i) {
            Material* material = frames[i].mMaterial;
            if (material && !material->IsUserAllocated()) {
                material->~Material();
                Layout::FreeMemory(material);
            }
            frames[i].mMaterial = nullptr;
        }
        Layout::FreeMemory(frames);
    }

    if (mContentMaterial && !mContentMaterial->IsUserAllocated()) {
        mContentMaterial->Finalize(device);
        Material* material = mContentMaterial;
        if (material) {
            material->~Material();
            Layout::FreeMemory(material);
        }
        mContentMaterial = nullptr;
    }
    mTexCoordArray.Free();
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
