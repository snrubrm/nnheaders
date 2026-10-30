#include <nn/atk/detail/SoundArchiveFileReader.h>
#include <nn/util/util_BytePtr.h>

#include <cstring>

namespace nn {
namespace atk {
namespace detail {

bool SoundArchiveFileReader::ReadBankInfo(u32 bank_id, SoundArchive::BankInfo* info) const {
    const auto* record = mInfoBlock->GetBankInfo(bank_id);
    if (!record)
        return false;
    info->mFileId = record->mItemFileId;
    return true;
}

bool SoundArchiveFileReader::ReadPlayerInfo(u32 player_id, SoundArchive::PlayerInfo* info) const {
    const auto* record = mInfoBlock->GetPlayerInfo(player_id);
    if (!record)
        return false;
    info->mSoundLimit = record->mPlayerSoundLimit;
    info->mHeapSize = record->GetPlayerHeapSize();
    return true;
}

bool SoundArchiveFileReader::ReadSoundGroupInfo(u32 sound_group_id,
                                                SoundArchive::SoundGroupInfo* info) const {
    const auto* record = mInfoBlock->GetSoundGroupInfo(sound_group_id);
    if (!record)
        return false;
    info->_0 = record->mSoundId;
    info->_4 = record->_4;
    info->_8 = nn::util::ConstBytePtr(record, record->mTableOffset).Get();
    return true;
}

SoundArchiveFileReader::SoundArchiveFileReader()
    : mStringBlock(nullptr), mInfoBlock(nullptr), _48(0) {
}

void SoundArchiveFileReader::Initialize(const void* file) {
    const auto* header = static_cast<const SoundArchiveFile::FileHeader*>(file);
    if (header->GetSignature() == SoundArchiveFile::FileHeader::cSignature &&
        u32(header->GetVersion() - SoundArchiveFile::FileHeader::cMinVersion) <=
            SoundArchiveFile::FileHeader::cMaxVersion - SoundArchiveFile::FileHeader::cMinVersion)
        std::memcpy(static_cast<SoundArchiveFile::FileHeader*>(this), file,
                sizeof(SoundArchiveFile::FileHeader));
}

void SoundArchiveFileReader::Finalize() {
    mStringBlock = nullptr;
    mInfoBlock = nullptr;
    _48 = 0;
}

void SoundArchiveFileReader::SetStringBlock(const void* block) {
    mStringBlock = &static_cast<const SoundArchiveFile::StringBlock*>(block)->mBody;
}

void SoundArchiveFileReader::SetInfoBlock(const void* block) {
    mInfoBlock = &static_cast<const SoundArchiveFile::InfoBlock*>(block)->mBody;
}

s32 SoundArchiveFileReader::GetItemId(const char* label) const {
    if (!mStringBlock)
        return -1;
    return mStringBlock->GetItemIdImpl(SoundArchiveFile::StringBlockBody::Sections_Item, label);
}

const char* SoundArchiveFileReader::GetItemLabel(u32 item_id) const {
    if (!mStringBlock)
        return nullptr;
    const s32 string_id = mInfoBlock->GetItemStringId(item_id);
    if (string_id == -1)
        return nullptr;
    return mStringBlock->GetString(string_id);
}

u32 SoundArchiveFileReader::GetItemFileId(u32 item_id) const {
    return mInfoBlock->GetItemFileId(item_id);
}

u32 SoundArchiveFileReader::GetItemPrefetchFileId(u32 item_id) const {
    return mInfoBlock->GetItemPrefetchFileId(item_id);
}

u32 SoundArchiveFileReader::GetSoundType(u32 sound_id) const {
    const SoundArchiveFile::SoundInfo* info = mInfoBlock->GetSoundInfo(sound_id);
    if (!info)
        return 0;
    return info->GetSoundType();
}

u32 SoundArchiveFileReader::GetSoundCount() const {
    return *mInfoBlock->GetSoundInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetPlayerCount() const {
    return *mInfoBlock->GetPlayerInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetGroupCount() const {
    return *mInfoBlock->GetGroupInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetWaveArchiveCount() const {
    return *mInfoBlock->GetWaveArchiveInfoReferenceTable();
}

u32 SoundArchiveFileReader::GetFileCount() const {
    return *mInfoBlock->GetFileInfoReferenceTable();
}

}  // namespace detail
}  // namespace atk
}  // namespace nn
