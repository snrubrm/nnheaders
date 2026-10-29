#pragma once

#include <nn/atk/detail/GroupFile.h>

namespace nn::atk::detail {

struct GroupItemLocationInfo {
    u32 _0;
    const void* mFileAddress;
};

class GroupFileReader {
public:
    explicit GroupFileReader(const void* file);
    bool ReadGroupItemLocationInfo(GroupItemLocationInfo* info, u32 index) const;
    u32 GetGroupItemExCount() const;
    bool ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* info, u32 index) const;

private:
    const Util::ReferenceTable* mInfo;
    const void* mFileData;
    const Util::ReferenceTable* mInfoEx;
};
static_assert(sizeof(GroupFileReader) == 0x18);

}  // namespace nn::atk::detail
