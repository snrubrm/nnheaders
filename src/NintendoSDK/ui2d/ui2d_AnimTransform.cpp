#include <nn/ui2d/AnimTransform.h>

#include <nn/ui2d/Layout.h>
#include <nn/ui2d/Material.h>
#include <nn/ui2d/Group.h>
#include <nn/ui2d/Pane.h>
#include <nn/ui2d/ResourceAccessor.h>
#include <new>

namespace nn::ui2d {

// 0x7100ab4a50
void AnimTransformBasic::BindMaterial(Material* material) {
    const ResAnimationBlock* block = mRes;
    const u32* offsets = block->GetContentOffsets();
    for (u32 i = 0; i < block->contentCount; ++i) {
        const ResAnimationContent* content = block->GetContentAtOffset(offsets[i]);
        if (content->type == 1 && material->IsNameEqual(content->name)) {
            if (!BindMaterialImpl(material, content))
                break;
        }
    }
}

// 0x7100ab5be4
const char* AnimResource::GetTagName() const {
    if (mTagBlock)
        return reinterpret_cast<const char*>(mTagBlock) + mTagBlock->nameOffset;
    return nullptr;
}

// 0x7100ab5c00
u16 AnimResource::GetGroupCount() const {
    if (mTagBlock)
        return mTagBlock->groupCount;
    return 0;
}

// 0x7100ab5c18
const ResAnimationGroupRef* AnimResource::GetGroupArray() const {
    if (mTagBlock)
        return reinterpret_cast<const ResAnimationGroupRef*>(reinterpret_cast<const u8*>(mTagBlock) +
                                                             mTagBlock->groupArrayOffset);
    return nullptr;
}

// 0x7100ab5c34
bool AnimResource::IsDescendingBind() const {
    if (mTagBlock)
        return mTagBlock->flags & 1;
    return false;
}

// 0x7100ab5c50
u16 AnimResource::GetAnimationShareInfoCount() const {
    if (mShareBlock)
        return mShareBlock->shareInfoCount;
    return 0;
}

// 0x7100ab5c68
const void* AnimResource::GetAnimationShareInfoArray() const {
    if (mShareBlock)
        return reinterpret_cast<const u8*>(mShareBlock) + mShareBlock->shareInfoArrayOffset;
    return nullptr;
}

// 0x7100ab41ec
u16 AnimTransform::GetFrameSize() const {
    return mRes->frameSize;
}

// 0x7100ab41f8
void AnimTransform::UpdateFrame(f32) {}

// 0x7100ab41fc
void AnimTransform::SetEnabled(bool enabled) {
    mEnabled = enabled;
}

// 0x7100ab4208
bool AnimTransform::IsLoopData() const {
    return mRes->loop != 0;
}

// 0x7100ab421c
bool AnimTransform::IsWaitData() const {
    return mRes->frameSize == 0;
}

// 0x7100ab4230
AnimTransformBasic::AnimTransformBasic() : _28(nullptr), _30(nullptr), mBindCount(0), _3a(0) {}

// 0x7100ab4260
AnimTransformBasic::~AnimTransformBasic() {
    if (_30)
        Layout::FreeMemory(_30);
    if (_28)
        Layout::FreeMemory(_28);
}

// 0x7100ab42f4
void AnimTransformBasic::SetResource(gfx::Device* device, ResourceAccessor* accessor,
                                     const ResAnimationBlock* block) {
    SetResource(device, accessor, block, block->contentCount);
}

// 0x7100ab4304
// NON_MATCHING: array initialization size calculation and offset-address scheduling differ.
void AnimTransformBasic::SetResource(gfx::Device* device, ResourceAccessor* accessor,
                                     const ResAnimationBlock* block, u16 count) {
    mRes = block;
    _28 = nullptr;
    const u16 texture_count = block->textureCount;
    if (texture_count != 0) {
        void* memory = Layout::AllocateMemory(texture_count * sizeof(TextureInfo*), 4);
        _28 = memory ? new (memory) TextureInfo*[texture_count]() : nullptr;
        if (_28) {
            const u32* offsets = reinterpret_cast<const u32*>(block + 1);
            for (s32 i = 0; i < block->textureCount; ++i) {
                const char* name = reinterpret_cast<const char*>(offsets) + offsets[i];
                _28[i] = accessor->AcquireTexture(device, name);
            }
        }
    }

    void* memory = Layout::AllocateMemory(count * sizeof(Binding), 4);
    _30 = memory ? new (memory) Binding[count]() : nullptr;
    if (_30)
        _3a = count;
}

// 0x7100ab4f18
void AnimTransformBasic::UnbindAll() {
    mBindCount = 0;
}

// 0x7100ab4e60
void AnimTransformBasic::UnbindGroup(const Group* group) {
    for (const PaneLink& link : group->mPaneLinkList)
        UnbindPane(link.pane);
}

// 0x7100ab4eb4
// NON_MATCHING: the natural u16 decrement emits sub rather than wrapped addition.
void AnimTransformBasic::UnbindMaterial(const Material* material) {
    for (s32 i = 0; i < mBindCount; ++i) {
        if (_30[i].target != material)
            continue;
        if (i + 1 < mBindCount)
            _30[i] = _30[mBindCount - 1];
        --mBindCount;
        break;
    }
}

// 0x7100ab4cdc
// NON_MATCHING: loop/callback scheduling and the natural u16 decrement differ.
void AnimTransformBasic::UnbindPane(const Pane* pane) {
    const u16 count = mBindCount;
    for (s32 i = 0; i < count; ++i) {
        if (_30[i].target == pane) {
            if (i + 1 < mBindCount)
                _30[i] = _30[mBindCount - 1];
            --mBindCount;
            break;
        }

        const u16 user_data_count = pane->GetExtUserDataCount();
        for (s32 j = 0; j < user_data_count; ++j) {
            if (_30[i].target == &pane->GetExtUserDataArray()[j]) {
                if (i + 1 < mBindCount)
                    _30[i] = _30[mBindCount - 1];
                --mBindCount;
                break;
            }
        }
    }

    const u8 material_count = pane->GetMaterialCount();
    for (s32 i = 0; i < material_count; ++i) {
        if (Material* material = pane->GetMaterial(i))
            UnbindMaterial(material);
    }
}

// 0x7100ab5088
void AnimTransformBasic::AnimateMaterial(Material* material) {
    if (!mEnabled)
        return;
    for (s32 i = 0; i < mBindCount; ++i) {
        if (_30[i].target == material) {
            AnimateMaterialImpl(material, _30[i].content);
            break;
        }
    }
}

// 0x7100ab4fb4
void AnimTransformBasic::AnimatePane(Pane* pane) {
    if (!mEnabled)
        return;
    for (s32 i = 0; i < mBindCount; ++i) {
        if (_30[i].target == pane) {
            AnimatePaneImpl(pane, _30[i].content);
            const u8 material_count = pane->GetMaterialCount();
            for (s32 j = 0; j < material_count; ++j) {
                if (Material* material = pane->GetMaterial(j))
                    AnimateMaterial(material);
            }
            break;
        }
    }
}

}  // namespace nn::ui2d
