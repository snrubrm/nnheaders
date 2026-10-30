#include <nn/atk/detail/SoundArchiveFileReader.h>
#include <nn/util/util_BytePtr.h>

#include <cstring>

namespace nn {
namespace atk {
namespace detail {

// NON_MATCHING: the version-dependent default uses the opposite branch layout.
bool SoundArchiveFileReader::ReadSoundInfo(u32 sound_id, SoundArchive::SoundInfo* info) const {
    const auto* record = mInfoBlock->GetSoundInfo(sound_id);
    if (!record)
        return false;
    info->mFileId = record->mItemFileId;
    info->_4 = record->_4;
    info->_a = record->_8;
    info->mPanMode = record->GetPanMode();
    info->mPanCurve = record->GetPanCurve();
    info->mPlayerPriority = record->GetPlayerPriority();
    info->mActorPlayerId = record->GetActorPlayerId();
    info->_b = record->_9;
    info->mFrontBypass = record->IsFrontBypass();
    return true;
}

bool SoundArchiveFileReader::ReadAdvancedWaveSoundInfo(
    u32 sound_id, SoundArchive::AdvancedWaveSoundInfo* info) const {
    const auto* sound = mInfoBlock->GetSoundInfo(sound_id);
    if (!sound || sound->GetSoundType() != 3)
        return false;
    info->_0 = sound->GetAdvancedWaveSoundInfo()->_0;
    return true;
}

bool SoundArchiveFileReader::ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info) const {
    const auto* record = mInfoBlock->GetSoundArchivePlayerInfo();
    if (!record)
        return false;
    info->sequenceSoundCount = record->mSequenceSoundCount;
    info->sequenceTrackCount = record->mSequenceTrackCount;
    info->streamSoundCount = record->mStreamSoundCount;
    info->streamTrackCount = record->mStreamTrackCount;
    info->streamChannelCount = record->mStreamChannelCount;
    info->waveSoundCount = record->mWaveSoundCount;
    info->waveTrackCount = record->mWaveTrackCount;
    info->streamBufferTimes = GetVersion() < 0x20200 ? 1 : record->mStreamBufferTimes;
    info->isAdvancedWaveSoundEnabled = (record->mFlags & 1) != 0;
    return true;
}

bool SoundArchiveFileReader::ReadStreamSoundInfo2(u32 id, SoundArchive::StreamSoundInfo2* info) const {
    const auto* sound = mInfoBlock->GetSoundInfo(id);
    if (!sound || sound->GetSoundType() != 2)
        return false;
    const auto* extension = sound->GetStreamSoundInfo()->GetStreamSoundExtension();
    if (!extension)
        return false;
    info->_0 = extension->_1 == 1;
    info->_4 = extension->_4;
    info->_8 = extension->_8;
    return true;
}

bool SoundArchiveFileReader::ReadWaveArchiveInfo(u32 id, SoundArchive::WaveArchiveInfo* info) const {
    const auto* record = mInfoBlock->GetWaveArchiveInfo(id);
    if (!record)
        return false;
    info->mFileId = record->mItemFileId;
    info->mWaveCount = record->GetWaveCount();
    info->_8 = record->_4;
    return true;
}

bool SoundArchiveFileReader::ReadSequenceSoundInfo(u32 id, SoundArchive::SequenceSoundInfo* info) const {
    const auto* sound = mInfoBlock->GetSoundInfo(id);
    if (!sound || sound->GetSoundType() != 1)
        return false;
    const auto* record = sound->GetSequenceSoundInfo();
    info->mStartOffset = record->GetStartOffset();
    info->_14 = record->_8;
    info->mChannelPriority = record->GetChannelPriority();
    info->mReleasePriorityFix = record->IsReleasePriorityFix();
    record->GetBankIds(info->mBankIds);
    return true;
}

bool SoundArchiveFileReader::ReadWaveSoundInfo(u32 id, SoundArchive::WaveSoundInfo* info) const {
    const auto* sound = mInfoBlock->GetSoundInfo(id);
    if (!sound || sound->GetSoundType() != 3)
        return false;
    const auto* record = sound->GetWaveSoundInfo();
    info->_0 = record->_0;
    info->_4 = record->_4;
    info->mChannelPriority = record->GetChannelPriority();
    info->mReleasePriorityFix = record->GetIsReleasePriorityFix() != 0;
    return true;
}

bool SoundArchiveFileReader::ReadGroupInfo(u32 group_id, SoundArchive::GroupInfo* info) const {
    const auto* group = mInfoBlock->GetGroupInfo(group_id);
    if (!group)
        return false;
    info->mFileId = group->mItemFileId;
    const auto* file = mInfoBlock->GetFileInfo(info->mFileId);
    if (!file)
        return false;
    const u32 type = file->GetFileLocationType();
    if (type == 0) {
        info->mFileSize = file->GetInternalFileInfo()->mSize;
    } else if (type == 1) {
        file->GetExternalFileInfo();
        info->mFileSize = 0xffffffff;
    } else {
        return false;
    }
    return true;
}

bool SoundArchiveFileReader::ReadFileInfo(u32 file_id, SoundArchive::FileInfo* info, int) const {
    const auto* file = mInfoBlock->GetFileInfo(file_id);
    if (!file)
        return false;
    const u32 type = file->GetFileLocationType();
    if (type == 0) {
        const auto* internal = file->GetInternalFileInfo();
        info->mFileSize = internal->mSize;
        info->mFileOffset = internal->mOffset == -1 ? -1 : internal->mOffset + 8;
    } else if (type == 1) {
        info->_8 = file->GetExternalFileInfo();
    } else {
        return false;
    }
    return true;
}

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
