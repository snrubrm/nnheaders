/**
 * @file AnimTransform.h
 * @brief Layout animations.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::ui2d {

class Group;
class Material;
class Pane;
class ResourceAccessor;
struct ResAnimationContent;
struct ResExtUserData;

// Resource blocks (offsets read by the accessors below).
struct ResAnimationBlock {
    u8 _0[8];
    u16 frameSize;
    u8 loop;
};

// The tag block of an animation resource (name, group list, flags).
struct ResAnimationTagBlock {
    u8 _0[0xa];
    u16 groupCount;
    u32 nameOffset;
    u32 groupArrayOffset;
    u8 _14[8];
    u8 flags;  // bit 0: descending bind
};

struct ResAnimationShareBlock {
    u8 _0[8];
    u32 shareInfoArrayOffset;
    u16 shareInfoCount;
};

struct ResAnimationGroupRef {
    char name[0x20];
    u32 _20;
};
static_assert(sizeof(ResAnimationGroupRef) == 0x24);

// Accessors of an animation resource set (0x7100ab5ad0 sets the two block pointers; only the part used by the accessors
// is known).
class AnimResource {
public:
    const char* GetTagName() const;
    u16 GetGroupCount() const;
    const ResAnimationGroupRef* GetGroupArray() const;
    bool IsDescendingBind() const;
    u16 GetAnimationShareInfoCount() const;
    const void* GetAnimationShareInfoArray() const;

private:
    /* 0x00 */ u8 _0[0x10];
    /* 0x10 */ const ResAnimationTagBlock* mTagBlock;
    /* 0x18 */ const ResAnimationShareBlock* mShareBlock;
};

// Layout evidence: AnimTransformBasic::AnimTransformBasic (0x7100ab4230) and the accessors (0x7100ab41ec ..). Slots of
// the vtable (0x249de58): GetRuntimeTypeInfo, destructor, UpdateFrame, SetEnabled, then AnimTransformBasic's.
class AnimTransform {
public:
    NN_RUNTIME_TYPEINFO_BASE()

    AnimTransform() : mRes(nullptr), mFrame(0.0f), mEnabled(true) {}
    virtual ~AnimTransform() = default;
    virtual void UpdateFrame(f32 frame);
    virtual void SetEnabled(bool enabled);
    virtual void Animate() = 0;
    virtual void AnimatePane(Pane* pane) = 0;
    virtual void AnimateMaterial(Material* material) = 0;
    virtual void SetResource(gfx::Device* device, ResourceAccessor* accessor,
                             const ResAnimationBlock* block) = 0;
    virtual void SetResource(gfx::Device* device, ResourceAccessor* accessor,
                             const ResAnimationBlock* block, u16 count) = 0;
    virtual void BindPane(Pane* pane, bool recursive) = 0;
    virtual void BindGroup(Group* group) = 0;
    virtual void BindMaterial(Material* material) = 0;
    virtual void ForceBindPane(Pane* pane, const Pane* source) = 0;
    virtual void UnbindPane(const Pane* pane) = 0;
    virtual void UnbindGroup(const Group* group) = 0;
    virtual void UnbindMaterial(const Material* material) = 0;
    virtual void UnbindAll() = 0;

    u16 GetFrameSize() const;
    bool IsLoopData() const;
    bool IsWaitData() const;

    util::IntrusiveListNode mLink;
    const ResAnimationBlock* mRes;
    f32 mFrame;
    bool mEnabled;
};

class AnimTransformBasic : public AnimTransform {
public:
    NN_RUNTIME_TYPEINFO(AnimTransform)

    AnimTransformBasic();
    ~AnimTransformBasic() override;

    void Animate() override;
    void AnimatePane(Pane* pane) override;
    void AnimateMaterial(Material* material) override;
    void SetResource(gfx::Device* device, ResourceAccessor* accessor,
                     const ResAnimationBlock* block) override;
    void SetResource(gfx::Device* device, ResourceAccessor* accessor, const ResAnimationBlock* block,
                     u16 count) override;
    void BindPane(Pane* pane, bool recursive) override;
    void BindGroup(Group* group) override;
    void BindMaterial(Material* material) override;
    void ForceBindPane(Pane* pane, const Pane* source) override;
    void UnbindPane(const Pane* pane) override;
    void UnbindGroup(const Group* group) override;
    void UnbindMaterial(const Material* material) override;
    void UnbindAll() override;

    virtual void AnimatePaneImpl(Pane* pane, const ResAnimationContent* content);
    virtual void AnimateMaterialImpl(Material* material, const ResAnimationContent* content);
    virtual void AnimateExtUserDataImpl(ResExtUserData* data, const ResAnimationContent* content);

protected:
    // SetResource allocates an array of 16-byte records. The target is a pane,
    // material or extended user-data entry; each bind stores it with its content.
    struct Binding {
        const void* target;
        const ResAnimationContent* content;
    };
    static_assert(sizeof(Binding) == 0x10);

    /* 0x28 */ void* _28;  // allocations released by the destructor
    /* 0x30 */ Binding* _30;
    /* 0x38 */ u16 mBindCount;
    /* 0x3a */ u16 _3a;
};
static_assert(sizeof(AnimTransform) == 0x28);
static_assert(sizeof(AnimTransformBasic) == 0x40);

}  // namespace nn::ui2d
