#include <nn/ui2d/Layout.h>

#include <new>
#include <nn/ui2d/TextBox.h>

namespace nn::ui2d {

// 0x7100ab79a8 (placeholder name; the CSV row is unnamed): sets the tag processor of every text box in the pane tree
void sub_7100AB79A8(Pane* pane, font::TagProcessorBase<u16>* tag_processor) {
    if (TextBox* text_box = font::DynamicCast<TextBox>(pane))
        text_box->SetTagProcessor(tag_processor);
    for (Pane& child : pane->GetChildList())
        sub_7100AB79A8(&child, tag_processor);
}

Layout::AllocateFunction Layout::g_pAllocateFunction;
Layout::FreeFunction Layout::g_pFreeFunction;
void* Layout::g_pUserData;

// 0x7100ab65cc
void Layout::SetAllocator(AllocateFunction allocate, FreeFunction free, void* user_data) {
    g_pAllocateFunction = allocate;
    g_pFreeFunction = free;
    g_pUserData = user_data;
}

// 0x7100ab79a0
void Layout::SetTagProcessor(font::TagProcessorBase<u16>* tag_processor) {
    sub_7100AB79A8(mPane, tag_processor);
}

// NON_MATCHING: same stores; the original stores the list node link together with the vtable and the root pane
// pointer together with the node's second link
// 0x7100ab662c
Layout::Layout() : mPane(nullptr), _20(nullptr), mName(nullptr), _40(0), _58(0) {}

// The original keeps the vtable store (a plain empty body drops it); `{ ; }` as in upstream's
// GameDataFlagSelector::~GameDataFlagSelector (commit 96101229).
// 0x7100ab665c
Layout::~Layout() {
    ;
}

// 0x7100ab711c
AnimTransform* Layout::CreateAnimTransformBasic() {
    void* memory = AllocateMemory(sizeof(AnimTransformBasic), 4);
    AnimTransformBasic* anim = nullptr;
    if (memory) {
        anim = new (memory) AnimTransformBasic();
        mAnimTransformList.push_back(*anim);
    }
    return anim;
}

// 0x7100ab7190
void Layout::DeleteAnimTransform(AnimTransform* anim) {
    mAnimTransformList.erase(mAnimTransformList.iterator_to(*anim));
    if (anim) {
        anim->~AnimTransform();
        FreeMemory(anim);
    }
}

// 0x7100ab7274
void Layout::BindAnimation(AnimTransform* anim) {
    if (mPane)
        anim->BindPane(mPane, true);
}

// 0x7100ab7298
void Layout::UnbindAnimation(AnimTransform* anim) {
    anim->UnbindAll();
}

// 0x7100ab72fc
void Layout::UnbindAllAnimation() {
    for (AnimTransform& anim : mAnimTransformList)
        anim.UnbindAll();
}

// 0x7100ab77c4
void Layout::Animate() {
    for (AnimTransform& anim : mAnimTransformList)
        anim.Animate();
    for (PartsLayoutLink& parts : mPartsLayoutList)
        parts.layout->Animate();
}

// 0x7100ab7840
void Layout::UpdateAnimFrame(f32 frame) {
    for (AnimTransform& anim : mAnimTransformList)
        anim.UpdateFrame(frame);
    for (PartsLayoutLink& parts : mPartsLayoutList)
        parts.layout->UpdateAnimFrame(frame);
}

// 0x7100ab78d0
void Layout::AnimateAndUpdateAnimFrame(f32 frame) {
    for (AnimTransform& anim : mAnimTransformList) {
        anim.Animate();
        anim.UpdateFrame(frame);
    }
    for (PartsLayoutLink& parts : mPartsLayoutList)
        parts.layout->AnimateAndUpdateAnimFrame(frame);
}

// 0x7100ab7f68
void Layout::Calculate(DrawInfo& draw_info, bool is_dirty) {
    CalculateImpl(draw_info, is_dirty);
}

// 0x7100ab65f4
void* Layout::AllocateMemory(size_t size, size_t alignment) {
    return g_pAllocateFunction(size, alignment, g_pUserData);
}

// 0x7100ab6610
void Layout::FreeMemory(void* ptr) {
    g_pFreeFunction(ptr, g_pUserData);
}

}  // namespace nn::ui2d
