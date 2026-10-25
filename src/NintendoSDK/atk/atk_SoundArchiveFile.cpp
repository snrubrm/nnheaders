#include <nn/atk/SoundArchive.h>
#include <nn/util/util_BytePtr.h>

namespace nn {
namespace atk {
namespace detail {

// Every accessor below resolves a relative offset that the file format stores in the record; the
// table getters return the table body, whose first word is its record count.
const u32* SoundArchiveFile::InfoBlockBody::GetSoundInfoReferenceTable() const {
    return nn::util::ConstBytePtr(this, mSoundInfoTableOffset).Get<u32>();
}

const u32* SoundArchiveFile::InfoBlockBody::GetPlayerInfoReferenceTable() const {
    return nn::util::ConstBytePtr(this, mPlayerInfoTableOffset).Get<u32>();
}

const u32* SoundArchiveFile::InfoBlockBody::GetSoundGroupInfoReferenceTable() const {
    return nn::util::ConstBytePtr(this, mSoundGroupInfoTableOffset).Get<u32>();
}

const u32* SoundArchiveFile::InfoBlockBody::GetGroupInfoReferenceTable() const {
    return nn::util::ConstBytePtr(this, mGroupInfoTableOffset).Get<u32>();
}

const u32* SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfoReferenceTable() const {
    return nn::util::ConstBytePtr(this, mWaveArchiveInfoTableOffset).Get<u32>();
}

const u32* SoundArchiveFile::InfoBlockBody::GetFileInfoReferenceTable() const {
    return nn::util::ConstBytePtr(this, mFileInfoTableOffset).Get<u32>();
}

// A reference table: the record count, then one 8-byte item per record whose second word is the
// record's offset from the table.
struct ReferenceTableItem {
    u32 _0;
    s32 offset;
};

struct ReferenceTable {
    u32 count;
    ReferenceTableItem items[1];
};

const void* SoundArchiveFile::InfoBlockBody::findRecord(const u32* body, u32 id) const {
    const auto* table = static_cast<const ReferenceTable*>(nn::util::ConstBytePtr(body).Get());
    const u32 index = id & 0xffffff;
    if (index >= table->count)
        return nullptr;
    return nn::util::ConstBytePtr(body, table->items[index].offset).Get();
}

const SoundArchiveFile::SoundInfo* SoundArchiveFile::InfoBlockBody::GetSoundInfo(u32 sound_id)
    const {
    if (sound_id >> 24 != 1)
        return nullptr;
    return static_cast<const SoundInfo*>(findRecord(GetSoundInfoReferenceTable(), sound_id));
}

const SoundArchiveFile::BankInfo* SoundArchiveFile::InfoBlockBody::GetBankInfo(u32 bank_id) const {
    if (bank_id >> 24 != 3)
        return nullptr;
    return static_cast<const BankInfo*>(findRecord(
        nn::util::ConstBytePtr(this, mBankInfoTableOffset).Get<u32>(), bank_id));
}

const SoundArchiveFile::PlayerInfo* SoundArchiveFile::InfoBlockBody::GetPlayerInfo(
    u32 player_id) const {
    if (player_id >> 24 != 4)
        return nullptr;
    return static_cast<const PlayerInfo*>(findRecord(GetPlayerInfoReferenceTable(), player_id));
}

const SoundArchiveFile::SoundGroupInfo* SoundArchiveFile::InfoBlockBody::GetSoundGroupInfo(
    u32 sound_group_id) const {
    if (sound_group_id >> 24 != 2)
        return nullptr;
    return static_cast<const SoundGroupInfo*>(
        findRecord(GetSoundGroupInfoReferenceTable(), sound_group_id));
}

const SoundArchiveFile::GroupInfo* SoundArchiveFile::InfoBlockBody::GetGroupInfo(u32 group_id)
    const {
    if (group_id >> 24 != 6)
        return nullptr;
    return static_cast<const GroupInfo*>(findRecord(GetGroupInfoReferenceTable(), group_id));
}

const SoundArchiveFile::WaveArchiveInfo* SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfo(
    u32 wave_archive_id) const {
    if (wave_archive_id >> 24 != 5)
        return nullptr;
    return static_cast<const WaveArchiveInfo*>(
        findRecord(GetWaveArchiveInfoReferenceTable(), wave_archive_id));
}

const SoundArchiveFile::FileInfo* SoundArchiveFile::InfoBlockBody::GetFileInfo(u32 file_id) const {
    return static_cast<const FileInfo*>(findRecord(GetFileInfoReferenceTable(), file_id));
}

const SoundArchiveFile::StreamSoundInfo* SoundArchiveFile::SoundInfo::GetStreamSoundInfo() const {
    return nn::util::ConstBytePtr(this, mSubInfoOffset).Get<StreamSoundInfo>();
}

const SoundArchiveFile::WaveSoundInfo* SoundArchiveFile::SoundInfo::GetWaveSoundInfo() const {
    return nn::util::ConstBytePtr(this, mSubInfoOffset).Get<WaveSoundInfo>();
}

const SoundArchiveFile::SequenceSoundInfo* SoundArchiveFile::SoundInfo::GetSequenceSoundInfo()
    const {
    return nn::util::ConstBytePtr(this, mSubInfoOffset).Get<SequenceSoundInfo>();
}

const SoundArchiveFile::AdvancedWaveSoundInfo*
SoundArchiveFile::SoundInfo::GetAdvancedWaveSoundInfo() const {
    return nn::util::ConstBytePtr(this, mSubInfoOffset).Get<AdvancedWaveSoundInfo>();
}

// The signature at +0xc selects the sound type; 0x2204 and later are not sound records.
u32 SoundArchiveFile::SoundInfo::GetSoundType() const {
    switch (mSignature) {
    case 0x2201:
        return 2;
    case 0x2202:
        return 3;
    case 0x2203:
        return 1;
    default:
        return 0;
    }
}

}  // namespace detail
}  // namespace atk
}  // namespace nn
