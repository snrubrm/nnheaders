#include <nn/ui2d/Pane.h>

#include <cstring>
#include <nn/ui2d/ResExtUserData.h>
#include <nn/ui2d/BuildTypes.h>
#include <nn/ui2d/Layout.h>
#include <nn/ui2d/Parts.h>
#include <nn/util/util_StringUtil.h>

namespace nn::ui2d {

// 0x7100ab8314
void Pane::SetName(const char* name) {
    util::Strlcpy(mPanelName, name, sizeof(mPanelName));
}

// 0x7100ab8578
// NON_MATCHING: dynamic-cast result branch materialization.
void Pane::CalculateScaleFromPartsRoot(util::Float2* scale, Pane* parent) const {
    scale->x = 1.0f;
    scale->y = 1.0f;
    while (parent) {
        if (font::DynamicCast<Parts>(parent))
            break;
        scale->x *= parent->mScale.x;
        scale->y *= parent->mScale.y;
        parent = parent->mParent;
    }
}

// 0x7100ab7f7c
// NON_MATCHING: list initialization, field-copy and parts-bound calculation scheduling.
Pane::Pane(const ResPane* resource, const BuildArgSet& args)
    : mParent(nullptr), mFlags(0), mFlagEx(0),
      mMtx{{{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}}},
      mUserMtx(nullptr), mExtUserDataList(nullptr), mAnimExtUserData(nullptr) {
    const util::Float3* translation = &resource->mTranslation;
    const util::Float3* rotation = &resource->mRotation;
    const util::Float2* scale = &resource->mScale;
    const Size* size = &resource->mSize;
    const u8* alpha = &resource->mAlpha;
    const char* user_data = resource->mUserData;
    if (args.mPaneOverride) {
        if (args.mPaneOverrideFlags & 0x8)
            translation = &args.mPaneOverride->mTranslation;
        if (args.mPaneOverrideFlags & 0x40)
            rotation = &args.mPaneOverride->mRotation;
        if (args.mPaneOverrideFlags & 0x20)
            scale = &args.mPaneOverride->mScale;
        if (args.mPaneOverrideFlags & 0x10)
            size = &args.mPaneOverride->mSize;
        if (args.mPaneOverrideFlags & 0x80)
            alpha = &args.mPaneOverride->mAlpha;
        if (args.mPaneOverrideFlags & 0x4)
            user_data = args.mPaneOverride->mUserData;
    }

    mBasePosition = resource->mBasePosition;
    SetName(resource->mName);
    util::Strlcpy(mUserData, user_data, sizeof(mUserData));
    mFlagEx = resource->mFlagsEx;
    if ((mFlagEx & (1 << PaneFlagEx_ExtUserDataAnimationEnabled)) && args.mExtUserData) {
        mAnimExtUserData = static_cast<ResExtUserDataList*>(
            Layout::AllocateMemory(args.mExtUserData->blockHeader.size, 4));
        std::memcpy(mAnimExtUserData, args.mExtUserData, args.mExtUserData->blockHeader.size);
        mExtUserDataList = mAnimExtUserData;
    } else {
        mExtUserDataList = args.mExtUserData;
    }
    mPosition = *translation;
    mRotation = *rotation;
    mScale = *scale;
    if (args.mOverrideResources &&
        (args.mPartsScale.x != 1.0f || args.mPartsScale.y != 1.0f) &&
        !(mFlagEx & (1 << PaneFlagEx_IgnorePartsMagnify))) {
        if (mFlagEx & (1 << PaneFlagEx_PartsMagnifyAdjustToPartsBound)) {
            const f32 horizontal = GetBasePositionH() == HorizontalPosition_Center ? 1.0f : 0.5f;
            const f32 vertical = GetBasePositionV() == VerticalPosition_Center ? 1.0f : 0.5f;
            util::Float2 root_scale;
            CalculateScaleFromPartsRoot(&root_scale, args.mParentPane);
            mSize.width = size->width +
                          horizontal * (args.mRootSize.x * args.mPartsScale.x - args.mRootSize.x) /
                              (root_scale.x * scale->x);
            mSize.height = size->height +
                           vertical * (args.mRootSize.y * args.mPartsScale.y - args.mRootSize.y) /
                               (root_scale.y * scale->y);
        } else {
            mSize.width = size->width * args.mPartsScale.x;
            mSize.height = size->height * args.mPartsScale.y;
        }
    } else {
        mSize = *size;
    }
    mAlpha = *alpha;
    mGlobalAlpha = mAlpha;
    mFlags = resource->mFlags;
    if (args.mOverrideResources && (args.mPaneOverrideFlags & 1))
        SetVisible((args.mPaneOverrideFlags & 2) != 0);
    SetGlobalMatrixDirty(true);
}

// 0x7100ab89e0
void Pane::AppendChild(Pane* child) {
    mChildList.push_back(*child);
    child->mParent = this;
    child->SetGlobalMatrixDirty(true);
}

// 0x7100ab8a14
void Pane::PrependChild(Pane* child) {
    mChildList.insert(mChildList.begin(), *child);
    child->mParent = this;
    child->SetGlobalMatrixDirty(true);
}

// 0x7100ab8a48
void Pane::InsertChild(Pane* position, Pane* child) {
    mChildList.insert(mChildList.iterator_to(*position), *child);
    child->mParent = this;
    child->SetGlobalMatrixDirty(true);
}

// 0x7100ab8a78
void Pane::RemoveChild(Pane* child) {
    mChildList.erase(mChildList.iterator_to(*child));
    child->mParent = nullptr;
}

// 0x7100ab7f78
Pane::~Pane() {}

// 0x7100ab8be4
Pane* Pane::FindPaneByName(const char* name, bool recursive) {
    if (IsNameEqual(name))
        return this;
    if (recursive) {
        for (Pane& child : mChildList) {
            if (Pane* found = child.FindPaneByNameRecursive(name))
                return found;
        }
    }
    return nullptr;
}

const Pane* Pane::FindPaneByName(const char* name, bool recursive) const {
    return const_cast<Pane*>(this)->FindPaneByName(name, recursive);
}

// 0x7100ab8ccc
const Material* Pane::FindMaterialByName(const char* name, bool recursive) const {
    return const_cast<Pane*>(this)->FindMaterialByName(name, recursive);
}

// 0x7100ab9414
void Pane::DrawSelf(DrawInfo&, gfx::CommandBuffer&) {}

// 0x7100ab9418
void Pane::BindAnimation(AnimTransform* transform, bool recursive, bool enabled) {
    transform->BindPane(this, recursive);
    transform->SetEnabled(enabled);
}

// 0x7100ab9464
void Pane::UnbindAnimation(AnimTransform* transform, bool recursive) {
    UnbindAnimationSelf(transform);
    if (recursive) {
        for (Pane& child : mChildList)
            child.UnbindAnimation(transform, true);
    }
}

// 0x7100ab94d4
void Pane::UnbindAnimationSelf(AnimTransform* transform) {
    const u8 count = GetMaterialCount();
    for (s32 i = 0; i < count; ++i) {
        if (Material* material = GetMaterial(i))
            transform->UnbindMaterial(material);
    }
    transform->UnbindPane(this);
}

// 0x7100ab9590
Material* Pane::GetMaterial() const {
    if (GetMaterialCount() == 0)
        return nullptr;
    return GetMaterial(0);
}

// 0x7100ab95e0
u8 Pane::GetMaterialCount() const {
    return 0;
}

// 0x7100ab95e8
Material* Pane::GetMaterial(s32) const {
    GetMaterialCount();  // result unused (in the target)
    return nullptr;
}

// 0x7100ab9758
Pane* Pane::FindPaneByNameRecursive(const char* name) {
    if (IsNameEqual(name))
        return this;
    for (Pane& child : mChildList) {
        if (Pane* found = child.FindPaneByNameRecursive(name))
            return found;
    }
    return nullptr;
}

const Pane* Pane::FindPaneByNameRecursive(const char* name) const {
    return const_cast<Pane*>(this)->FindPaneByNameRecursive(name);
}

// 0x7100ab982c
const Material* Pane::FindMaterialByNameRecursive(const char* name) const {
    return const_cast<Pane*>(this)->FindMaterialByNameRecursive(name);
}

// 0x7100ab8aac
util::Float2 Pane::GetVertexPos() const {
    util::Float2 pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    switch (GetBasePositionH()) {
    case HorizontalPosition_Center:
        pos.x = mSize.width * -0.5f;
        break;
    case HorizontalPosition_Right:
        pos.x = -mSize.width;
        break;
    default:
        break;
    }
    switch (GetBasePositionV()) {
    case VerticalPosition_Center:
        pos.y = mSize.height * 0.5f;
        break;
    case VerticalPosition_Bottom:
        pos.y = mSize.height;
        break;
    default:
        break;
    }
    return pos;
}

// 0x7100ab8b08
util::Unorm8x4 Pane::GetVertexColor(s32) const {
    util::Unorm8x4 color;
    for (u8& v : color.v)
        v = 0xff;
    return color;
}

// 0x7100ab8b10
void Pane::SetVertexColor(s32, const util::Unorm8x4&) {}

// 0x7100ab8b14
u8 Pane::GetColorElement(s32 index) const {
    if (index == 16)
        return mAlpha;
    return GetVertexColorElement(index);
}

// 0x7100ab8b30
void Pane::SetColorElement(s32 index, u8 value) {
    if (index == 16)
        mAlpha = value;
    else
        SetVertexColorElement(index, value);
}

// 0x7100ab8b4c
u8 Pane::GetVertexColorElement(s32) const {
    return 0xff;
}

// 0x7100ab8b54
void Pane::SetVertexColorElement(s32, u8) {}

// 0x7100ab98d8
void Pane::m19(DrawInfo& draw_info, CalculateContext& context, bool is_dirty) {
    Calculate(draw_info, context, is_dirty);
}

// 0x7100ab9608
u16 Pane::GetExtUserDataCount() const {
    if (mExtUserDataList)
        return mExtUserDataList->count;
    return 0;
}

// 0x7100ab9620
const ResExtUserData* Pane::GetExtUserDataArray() const {
    if (detail::TestBit(mFlagEx, PaneFlagEx_ExtUserDataAnimationEnabled)) {
        if (mAnimExtUserData)
            return mAnimExtUserData->GetArray();
        return nullptr;
    }
    if (mExtUserDataList)
        return mExtUserDataList->GetArray();
    return nullptr;
}

// 0x7100ab9644
const ResExtUserData* Pane::FindExtUserDataByName(const char* name) const {
    const ResExtUserData* array = GetExtUserDataArray();
    if (array) {
        // The count always comes from the resource's list, also when the (animated) copy supplies the entries.
        const s32 count = mExtUserDataList->count;
        const ResExtUserData* data = array;
        for (s32 i = 0; i < count; i++, data++) {
            if (std::strcmp(name, data->GetName()) == 0)
                return data;
        }
    }
    return nullptr;
}

}  // namespace nn::ui2d
