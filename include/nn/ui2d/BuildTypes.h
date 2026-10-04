#pragma once

#include <nn/types.h>
#include <nn/ui2d/Layout.h>
#include <nn/util/MathTypes.h>
#include <cstddef>

namespace nn::ui2d {

class ControlCreator;
class TextSearcher;
struct ResTextureList;
struct ResFontList;
struct ResMaterialList;
struct ResMaterial;
struct ResPaneBasicInfo;
struct ResExtUserDataList;
struct UserShaderInformation;
class Parts;
struct ResParts;

// BuildImpl (0xab6bcc) collects the three resource blocks and their owner.
// Resource list names and field names are descriptive guesses.
struct BuildResSet {
    const ResTextureList* mTextureList;
    const ResFontList* mFontList;
    const ResMaterialList* mMaterialList;
    ResourceAccessor* mResourceAccessor;
    Layout* mLayout;
};
static_assert(sizeof(BuildResSet) == 0x28);

namespace detail {
const ResMaterial* GetResMaterial(const BuildResSet*, u16 index);
}

// BuildPartsImpl constructs this 0x30-byte record at 0xab6b14. Its 0x28-byte
// resource entries select replacement pane data and extended user data.
struct Layout::PartsBuildDataSet {
    struct Entry {
        char mPaneName[24];
        u8 mTextOverrideFlags;
        u8 mPaneOverrideFlags;
        u8 _1a;
        u32 mPaneResourceOffset;
        u32 mExtUserDataOffset;
        u32 mPaneBasicInfoOffset;
    };
    s32 mCount;
    const Entry* mEntries;
    Parts* mPartsPane;
    const ResParts* mResource;
    const BuildResSet* mResources;
    util::Float2 mPartsScale;
};
static_assert(sizeof(Layout::PartsBuildDataSet::Entry) == 0x28);
static_assert(sizeof(Layout::PartsBuildDataSet) == 0x30);
static_assert(offsetof(Layout::PartsBuildDataSet, mPartsPane) == 0x10);

// BuildWithName (0xab67e4), BuildImpl, BuildPartsImpl (0xab68c0) and the
// resource Pane constructor (0xab7f7c) establish the complete argument record.
// The shader callback (0xac2cb8) fills UserShaderInformation from the resource's
// extended user data. No layout for that callback's result is assumed here.
struct BuildArgSet {
    util::Float2 mPartsScale;
    util::Float2 mRootSize;
    ControlCreator* mControlCreator;
    TextSearcher* mTextSearcher;
    const Layout* mParentLayout;
    Layout* mRootLayout;
    const BuildResSet* mResources;
    const BuildResSet* mOverrideResources;
    u16 mTextOverrideFlags;
    u16 mPaneOverrideFlags;
    u16 _44;
    const ResPaneBasicInfo* mPaneOverride;
    Pane* mParentPane;
    UserShaderCallback mUserShaderCallback;
    const ResExtUserDataList* mExtUserData;
    Layout::BuildOption mBuildOption;
    bool mIsUtf8;
};
static_assert(sizeof(BuildArgSet) == 0x70);
static_assert(offsetof(BuildArgSet, mTextSearcher) == 0x18);
static_assert(offsetof(BuildArgSet, mParentLayout) == 0x20);
static_assert(offsetof(BuildArgSet, mUserShaderCallback) == 0x58);
static_assert(offsetof(BuildArgSet, mIsUtf8) == 0x69);

// Material construction (0xac1b50) and text initialization (0xabaae8) add
// aligned GPU byte sizes to these two counters. Names are descriptive guesses.
class BuildResultInformation {
public:
    size_t mMaterialBufferSize = 0;
    size_t mTextBufferSize = 0;
};
static_assert(sizeof(BuildResultInformation) == 0x10);

}  // namespace nn::ui2d
