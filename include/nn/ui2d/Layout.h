/**
 * @file Layout.h
 * @brief UI Layout implementation.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>
#include <nn/ui2d/AnimTransform.h>
#include <nn/ui2d/Types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace font {
template <typename CharType>
class TagProcessorBase;
}  // namespace font

namespace ui2d {
class AnimResource;
class DrawInfo;
class Pane;
class BuildResultInformation;
struct BuildArgSet;
struct BuildResSet;
class ResourceAccessor;
class ControlCreator;
class TextSearcher;
struct UserShaderInformation;
struct ResExtUserDataList;
using UserShaderCallback = bool (*)(UserShaderInformation*, const ResExtUserDataList*);

// An entry of a layout's list of parts layouts (the layouts of the parts panes; Layout::Animate etc. forward to them).
struct PartsLayoutLink {
    util::IntrusiveListNode link;
    class Layout* layout;
};

// Layout evidence: Layout::Layout (0x7100ab662c), Animate / UpdateAnimFrame (0x7100ab77c4 / 0x7100ab7840) and the
// vtable (0x249df68): slot 0 GetRuntimeTypeInfo, 1 / 2 destructor, then the virtuals below in this order.
class Layout {
public:
    struct PartsBuildDataSet;
    // A one-byte build option; BuildPartsImpl tests whether parts need a root pane.
    struct BuildOption {
        u8 mBuildPartsRoot = 0;
    };
    static_assert(sizeof(BuildOption) == 1);

    typedef void* (*AllocateFunction)(size_t size, size_t alignment, void* user_data);
    typedef void (*FreeFunction)(void* ptr, void* user_data);

    NN_RUNTIME_TYPEINFO_BASE()

    Layout();
    virtual ~Layout();

    virtual void DeleteAnimTransform(AnimTransform*);
    virtual void BindAnimation(AnimTransform*);
    virtual void UnbindAnimation(AnimTransform*);
    virtual void UnbindAnimation(Pane*);
    virtual void UnbindAllAnimation();
    virtual void BindAnimationAuto(gfx::Device*, const AnimResource&);
    virtual void Animate();
    virtual void UpdateAnimFrame(f32 frame);
    virtual void AnimateAndUpdateAnimFrame(f32 frame);
    // Slot 12: forwards to CalculateImpl (the bool is masked to one bit); slot 13: draws the root pane with this layout
    // set in the draw info. Names follow the NintendoWare layout library.
    virtual void Calculate(DrawInfo&, bool);
    virtual void Draw(DrawInfo&, gfx::CommandBuffer&);
    // Sets the tag processor of every text box of the layout's pane tree.
    virtual void SetTagProcessor(font::TagProcessorBase<u16>* tag_processor);
    virtual bool BuildImpl(BuildResultInformation*, gfx::Device*, const void*, ResourceAccessor*,
                           const BuildArgSet&, const PartsBuildDataSet*);
    virtual bool BuildPartsImpl(BuildResultInformation*, gfx::Device*, const void*,
                                const PartsBuildDataSet*, BuildArgSet&, BuildResSet&, u32);
    virtual Pane* BuildPaneObj(BuildResultInformation*, gfx::Device*, u32, const void*, const void*,
                               const BuildArgSet&);
    virtual bool BuildPartsLayout(BuildResultInformation*, gfx::Device*, const char*,
                                  const PartsBuildDataSet&, const BuildArgSet&);
    virtual void CalculateImpl(DrawInfo&, bool);

    static void SetAllocator(AllocateFunction, FreeFunction, void* user_data);
    // The callers pass the alignment (the CSV's one-argument name for 0x7100ab65f4 is an IDA guess).
    static void* AllocateMemory(size_t size, size_t alignment);
    static void FreeMemory(void* src);

    // The allocator set by SetAllocator (public: eui::GetNwAllocatorHeap returns the user data, which is the
    // game's heap).
    static AllocateFunction g_pAllocateFunction;
    static FreeFunction g_pFreeFunction;
    static void* g_pUserData;

    Pane* GetPane() const { return mPane; }

    AnimTransform* CreateAnimTransformBasic();
    bool BuildWithName(BuildResultInformation*, gfx::Device*, ResourceAccessor*, ControlCreator*,
                       TextSearcher*, const BuildOption&, const char* name, bool is_utf8);

protected:
    typedef util::IntrusiveList<AnimTransform,
                                util::IntrusiveListMemberNodeTraits<AnimTransform, &AnimTransform::mLink>>
        AnimTransformList;
    typedef util::IntrusiveList<PartsLayoutLink,
                                util::IntrusiveListMemberNodeTraits<PartsLayoutLink, &PartsLayoutLink::link>>
        PartsLayoutList;

    /* 0x08 */ AnimTransformList mAnimTransformList;
    /* 0x18 */ Pane* mPane;
    /* 0x20 */ void* _20;
    /* 0x28 */ Size mLayoutSize;
    /* 0x30 */ const char* mName;
    /* 0x38 */ u64 _38;
    /* 0x40 */ u64 _40;
    /* 0x48 */ PartsLayoutList mPartsLayoutList;
    /* 0x58 */ UserShaderCallback mUserShaderCallback;
};
static_assert(sizeof(Layout) == 0x60);

void sub_710132B114(Layout::AllocateFunction allocate, Layout::FreeFunction free, void* user_data);

}  // namespace ui2d
}  // namespace nn
