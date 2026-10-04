#include <nn/ui2d/ControlSrc.h>
#include <nn/util/util_BytePtr.h>

#include <cstring>

namespace nn::ui2d {

// 0x7100ac04e8
ControlSrc::ControlSrc(const ResControl* resource, const ResExtUserDataList* ext_user_data) {
    mClassName = resource->mClassName;
    mName = util::ConstBytePtr(resource, resource->mNameOffset).Get<char>();
    const u16 pane_count = resource->mPaneCount;
    mPaneCount = pane_count;
    mAnimCount = resource->mAnimCount;
    mPaneNames = util::ConstBytePtr(resource, resource->mPaneNamesOffset).Get<char[cPaneNameLength]>();
    mAnimNames = util::ConstBytePtr(resource, resource->mPaneNamesOffset +
                                           pane_count * cPaneNameLength).Get<u32>();
    mFunctionalPaneNames =
        util::ConstBytePtr(resource, resource->mFunctionalPaneNamesOffset).Get<u32>();
    mFunctionalAnimNames =
        util::ConstBytePtr(resource, resource->mFunctionalAnimNamesOffset).Get<u32>();
    mExtUserData = ext_user_data;
}

// 0x7100ac0548
const char* ControlSrc::FindFunctionalPaneName(const char* name) const {
    for (u32 i = 0; i < mPaneCount; ++i) {
        if (std::strcmp(util::ConstBytePtr(mFunctionalPaneNames, mFunctionalPaneNames[i]).Get<char>(),
                        name) == 0)
            return mPaneNames[i];
    }
    return nullptr;
}

// 0x7100ac05c0
const char* ControlSrc::FindFunctionalAnimName(const char* name) const {
    for (u32 i = 0; i < mAnimCount; ++i) {
        if (std::strcmp(util::ConstBytePtr(mFunctionalAnimNames, mFunctionalAnimNames[i]).Get<char>(),
                        name) == 0)
            return util::ConstBytePtr(mAnimNames, mAnimNames[i]).Get<char>();
    }
    return nullptr;
}

// 0x7100ac0634
const ResExtUserData* ControlSrc::FindExtUserDataByName(const char* name) const {
    if (!mExtUserData)
        return nullptr;
    const ResExtUserData* entry = mExtUserData->GetArray();
    for (s32 i = 0; i < mExtUserData->GetCount(); ++i, ++entry) {
        if (std::strcmp(name, entry->GetName()) == 0)
            return entry;
    }
    return nullptr;
}

}  // namespace nn::ui2d
