#include <nn/ui2d/AnimTransform.h>

#include <nn/ui2d/Layout.h>

namespace nn::ui2d {

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

// 0x7100ab4f18
void AnimTransformBasic::UnbindAll() {
    mBindCount = 0;
}

}  // namespace nn::ui2d
