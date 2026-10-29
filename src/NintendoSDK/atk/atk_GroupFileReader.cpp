#include <nn/atk/detail/GroupFileReader.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

GroupFileReader::GroupFileReader(const void* file)
    : mInfo(nullptr), mFileData(nullptr), mInfoEx(nullptr) {
    const auto* header = static_cast<const GroupFile::FileHeader*>(file);
    if (header->mSignature != 0x50524746 || header->mVersion != 0x10000)
        return;
    const auto* info = header->GetInfoBlock();
    const auto* data = header->GetFileBlock();
    const auto* infoEx = header->GetInfoExBlock();
    if (!info || !data || info->mSignature != 0x4f464e49 || data->mSignature != 0x454c4946)
        return;
    if (infoEx) {
        if (infoEx->mSignature != 0x58464e49)
            return;
        mInfoEx = &infoEx->mBody;
    }
    mInfo = &info->mBody;
    mFileData = data->mBody;
}

// NON_MATCHING: the nullable file address uses a branch rather than a conditional select.
bool GroupFileReader::ReadGroupItemLocationInfo(GroupItemLocationInfo* info, u32 index) const {
    if (!mInfo)
        return false;
    const auto* item = static_cast<const GroupFile::GroupItemInfo*>(mInfo->GetReferedItem(index));
    if (!item)
        return false;
    info->_0 = item->_0;
    info->mFileAddress = item->mFileReference.offset == -1
                             ? nullptr
                             : nn::util::ConstBytePtr(mFileData, item->mFileReference.offset).Get();
    return true;
}

u32 GroupFileReader::GetGroupItemExCount() const {
    return mInfoEx ? mInfoEx->count : 0;
}

bool GroupFileReader::ReadGroupItemInfoEx(GroupFile::GroupItemInfoEx* info, u32 index) const {
    if (!mInfoEx)
        return false;
    const auto* item = static_cast<const GroupFile::GroupItemInfoEx*>(mInfoEx->GetReferedItem(index));
    if (!item)
        return false;
    *info = *item;
    return true;
}

}  // namespace nn::atk::detail
