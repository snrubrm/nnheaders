#pragma once

#include <nn/types.h>
#include <nn/ui2d/ResExtUserData.h>

namespace nn::ui2d {

// The 'cnt1' resource header, consumed by the constructor at 0xac04e8.
// The class-name string and other resource data extend beyond this prefix.
struct ResControl {
    ResBlockHeader mBlockHeader;
    u32 mNameOffset;
    u32 mPaneNamesOffset;
    u16 mPaneCount;
    u16 mAnimCount;
    u32 mFunctionalPaneNamesOffset;
    u32 mFunctionalAnimNamesOffset;
    char mClassName[1];
};

// Actual 0x40-byte control source, built at 0xac04e8. Pane names are fixed
// 24-byte records; the three other name tables contain relative u32 offsets.
// Field and resource names are descriptive guesses.
class ControlSrc {
public:
    ControlSrc(const ResControl*, const ResExtUserDataList*);

    const char* FindFunctionalAnimName(const char* name) const;
    const char* FindFunctionalPaneName(const char* name) const;
    const ResExtUserData* FindExtUserDataByName(const char* name) const;

    static constexpr s32 cPaneNameLength = 24;

    const char* mClassName;
    const char* mName;
    u16 mPaneCount;
    u16 mAnimCount;
    const char (*mPaneNames)[cPaneNameLength];
    const u32* mAnimNames;
    const u32* mFunctionalPaneNames;
    const u32* mFunctionalAnimNames;
    const ResExtUserDataList* mExtUserData;
};
static_assert(sizeof(ControlSrc) == 0x40);

}  // namespace nn::ui2d
