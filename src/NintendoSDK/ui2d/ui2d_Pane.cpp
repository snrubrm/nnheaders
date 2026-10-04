#include <nn/ui2d/Pane.h>

#include <cstring>
#include <nn/ui2d/ResExtUserData.h>

namespace nn::ui2d {

// 0x7100ab7f78
Pane::~Pane() {}

// 0x7100ab8be4
const Pane* Pane::FindPaneByName(const char* name, bool recursive) const {
    return const_cast<Pane*>(this)->FindPaneByName(name, recursive);
}

// 0x7100ab8ccc
const Material* Pane::FindMaterialByName(const char* name, bool recursive) const {
    return const_cast<Pane*>(this)->FindMaterialByName(name, recursive);
}

// 0x7100ab9414
void Pane::DrawSelf(DrawInfo&, gfx::CommandBuffer&) {}

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
const Pane* Pane::FindPaneByNameRecursive(const char* name) const {
    return const_cast<Pane*>(this)->FindPaneByNameRecursive(name);
}

// 0x7100ab982c
const Material* Pane::FindMaterialByNameRecursive(const char* name) const {
    return const_cast<Pane*>(this)->FindMaterialByNameRecursive(name);
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
