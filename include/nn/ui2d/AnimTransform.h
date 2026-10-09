/**
 * @file AnimTransform.h
 * @brief Layout animations.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>
#include <nn/util/util_BytePtr.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::ui2d {

class Group;
class Material;
class Pane;
class ResourceAccessor;
class TextureInfo;
struct ResExtUserData;

// BindMaterial (0x7100ab4a50) bounds the name comparison to 28 bytes.
// AnimatePaneImpl/AnimateExtUserDataImpl read the count and type at +0x1c/+0x1d.
struct ResAnimationContent {
    char name[0x1c];
    u8 infoCount;
    u8 type;
    u8 _1e[2];

    // inline-only in the original; name is a guess. BindPane (0x7100ab4504),
    // BindGroup (0x7100ab49e4) and ForceBindPane (0x7100ab4b84) resolve this
    // type-2 trailer: content-relative table offset, then table-relative name.
    const char* GetExtUserDataName() const {
        if (type != 2)
            return nullptr;
        const u32* table_offset = util::ConstBytePtr(this, 0x24).Get<u32>();
        const u32* table = util::ConstBytePtr(this, *table_offset).Get<u32>();
        return util::ConstBytePtr(table, *table).Get<char>();
    }
};
static_assert(sizeof(ResAnimationContent) == 0x20);
static_assert(offsetof(ResAnimationContent, infoCount) == 0x1c);
static_assert(offsetof(ResAnimationContent, type) == 0x1d);

// Resource blocks (offsets read by the accessors below).
struct ResAnimationBlock {
    u8 _0[8];
    u16 frameSize;
    u8 loop;
    u16 textureCount;
    u16 contentCount;
    u32 contentOffsetsOffset;

    // inline-only in the original; names are guesses. BindPane (0x7100ab4428),
    // BindGroup (0x7100ab487c) and eui 0x7100aa205c resolve these two stages.
    const u32* GetContentOffsets() const {
        return util::ConstBytePtr(this, contentOffsetsOffset).Get<u32>();
    }
    const ResAnimationContent* GetContentAtOffset(u32 offset) const {
        return util::ConstBytePtr(this, offset).Get<ResAnimationContent>();
    }
};
static_assert(sizeof(ResAnimationBlock) == 0x14);
// Texture-name offsets immediately follow this fixed prefix. They are relative
// to that trailing table; content offsets are instead relative to the block.

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

// Accessors of an animation resource set; only the blocks used here are known.
class AnimResource {
public:
    void Set(const void* resource);

    // inline-only in the original; name is a guess. Layout::BindAnimationAuto and
    // eui::LayoutEx::tryCreateAnimatorAuto read this block before SetResource.
    const ResAnimationBlock* GetAnimationBlock() const { return mAnimationBlock; }

    const char* GetTagName() const;
    u16 GetGroupCount() const;
    const ResAnimationGroupRef* GetGroupArray() const;
    bool IsDescendingBind() const;
    u16 GetAnimationShareInfoCount() const;
    const void* GetAnimationShareInfoArray() const;

private:
    /* 0x00 */ u8 _0[8];
    /* 0x08 */ const ResAnimationBlock* mAnimationBlock;
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
    // Native 0x7100ab457c returns false at capacity and true after binding.
    bool BindMaterialImpl(Material* material, const ResAnimationContent* content);

    // SetResource allocates an array of 16-byte records. The target is a pane,
    // material or extended user-data entry; each bind stores it with its content.
    struct Binding {
        const void* target;
        const ResAnimationContent* content;
    };
    static_assert(sizeof(Binding) == 0x10);

    /* 0x28 */ TextureInfo** _28;  // texture references acquired from the resource accessor
    /* 0x30 */ Binding* _30;
    /* 0x38 */ u16 mBindCount;
    /* 0x3a */ u16 _3a;  // SetResource's allocated binding capacity
};
static_assert(sizeof(AnimTransform) == 0x28);
static_assert(sizeof(AnimTransformBasic) == 0x40);

}  // namespace nn::ui2d
