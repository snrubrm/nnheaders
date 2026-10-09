#include <nn/ui2d/Layout.h>

#include <new>
#include <nn/ui2d/BuildTypes.h>
#include <nn/ui2d/DrawInfo.h>
#include <nn/ui2d/Group.h>
#include <nn/ui2d/ResourceAccessor.h>
#include <nn/util.h>
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
Layout::Layout()
    : mPane(nullptr), _20(nullptr), mLayoutSize{}, mName(nullptr), mResourceAccessor(nullptr),
      mUserShaderCallback(nullptr) {}

// 0x7100ab6674
void Layout::Finalize(gfx::Device* device) {
    mPartsLayoutList.clear();
    if (_20) {
        GroupContainer* groups = _20;
        groups->~GroupContainer();
        FreeMemory(groups);
    }
    _20 = nullptr;
    if (mPane && !mPane->IsUserAllocated()) {
        mPane->Finalize(device);
        Pane* pane = mPane;
        if (pane) {
            pane->~Pane();
            FreeMemory(pane);
        }
        mPane = nullptr;
    }
    AnimTransformList::iterator it = mAnimTransformList.begin();
    while (it != mAnimTransformList.end()) {
        AnimTransformList::iterator current = it++;
        mAnimTransformList.erase(current);
        current->~AnimTransform();
        FreeMemory(&*current);
    }
    mLayoutSize = {};
    mName = nullptr;
    _38 = 0;
    mResourceAccessor = nullptr;
    mUserShaderCallback = nullptr;
}

// 0x7100ab67e4
bool Layout::BuildWithName(BuildResultInformation* result, gfx::Device* device,
                           ResourceAccessor* resource_accessor, ControlCreator* control_creator,
                           TextSearcher* text_searcher, const BuildOption& option,
                           const char* name, bool is_utf8) {
    const void* resource = resource_accessor->FindResourceByName(0x626c7974, name);
    if (!resource)
        return false;

    // BuildImpl collects the remaining resource and pane arguments itself.
    BuildArgSet args;
    args.mPartsScale = {{1.0f, 1.0f}};
    args.mRootSize = {{0.0f, 0.0f}};
    args.mControlCreator = control_creator;
    args.mTextSearcher = text_searcher;
    args.mParentLayout = nullptr;
    args.mRootLayout = this;
    args.mBuildOption = option;
    args.mIsUtf8 = is_utf8;
    args.mUserShaderCallback = mUserShaderCallback;
    return BuildImpl(result, device, resource, resource_accessor, args, nullptr);
}

// 0x7100ab7ebc
const void* Layout::GetLayoutResourceData(const char* name) const {
    char resource_name[72];
    util::SNPrintf(resource_name, sizeof(resource_name), "%s.bflyt", name);
    return mResourceAccessor->FindResourceByName(0x626c7974, resource_name);
}

// 0x7100ab7214
const void* Layout::GetAnimResourceData(const char* name) const {
    char resource_name[136];
    util::SNPrintf(resource_name, sizeof(resource_name), "%s_%s.bflan", mName, name);
    return mResourceAccessor->FindResourceByName(0x616e696d, resource_name);
}

// 0x7100ab7dc0
// NON_MATCHING: Layout constructor list initialization store grouping.
Layout* Layout::BuildPartsLayout(BuildResultInformation* result, gfx::Device* device,
                                const char* name, const PartsBuildDataSet& parts,
                                const BuildArgSet& args) {
    const void* resource = GetLayoutResourceData(name);
    Layout* layout = new (AllocateMemory(sizeof(Layout), 4)) Layout;
    layout->mUserShaderCallback = mUserShaderCallback;
    layout->BuildImpl(result, device, resource, mResourceAccessor, args, &parts);
    return layout;
}

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

// 0x7100ab72a8
void Layout::UnbindAnimation(Pane* pane) {
    for (AnimTransform& anim : mAnimTransformList)
        anim.UnbindPane(pane);
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

// 0x7100ab7700
// NON_MATCHING: the shared CalculateContext initializes fields absent from this SDK caller's stack setup.
void Layout::CalculateImpl(DrawInfo& draw_info, bool is_dirty) {
    if (!mPane)
        return;
    Pane::CalculateContext context;
    context.Set(draw_info, this);
    draw_info.mLayout = this;
    mPane->Calculate(draw_info, context, is_dirty);
    draw_info.mLayout = nullptr;
}

// 0x7100ab65f4
void* Layout::AllocateMemory(size_t size, size_t alignment) {
    return g_pAllocateFunction(size, alignment, g_pUserData);
}

// 0x7100ab6610
void Layout::FreeMemory(void* ptr) {
    g_pFreeFunction(ptr, g_pUserData);
}

// 0x7100ab7770
void Layout::Draw(DrawInfo& draw_info, gfx::CommandBuffer& command_buffer) {
    if (!mPane)
        return;
    draw_info._eb[4] = 1;
    draw_info._eb[0] = 6;
    draw_info._eb[1] = 0;
    draw_info._eb[2] = 0;
    draw_info.mLayout = this;
    mPane->Draw(draw_info, command_buffer);
    draw_info.mLayout = nullptr;
}

// 0x7100ab7b28
ShaderInfo* Layout::AcquireArchiveShader(gfx::Device* device, const char* name) const {
    return mResourceAccessor->AcquireShader(device, name);
}

}  // namespace nn::ui2d
