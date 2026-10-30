#include <nn/atk/SoundArchive.h>
#include <nn/atk/detail/Util.h>
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

const SoundArchiveFile::StreamSoundTrackInfo* SoundArchiveFile::StreamSoundInfo::GetTrackInfoTable()
    const {
    if (mTrackInfoSignature != 0x101)
        return nullptr;
    return nn::util::ConstBytePtr(this, mTrackInfoOffset).Get<StreamSoundTrackInfo>();
}

const SoundArchiveFile::StreamSoundExtension*
SoundArchiveFile::StreamSoundInfo::GetStreamSoundExtension() const {
    if (mExtensionOffset == -1)
        return nullptr;
    if (mExtensionSignature != 0x2210)
        return nullptr;
    return nn::util::ConstBytePtr(this, mExtensionOffset).Get<StreamSoundExtension>();
}

const u8* SoundArchiveFile::StreamSoundInfo::GetSendValue() const {
    return nn::util::ConstBytePtr(this, mSendValueOffset).Get<u8>();
}

const SoundArchiveFile::SoundArchivePlayerInfo*
SoundArchiveFile::InfoBlockBody::GetSoundArchivePlayerInfo() const {
    return nn::util::ConstBytePtr(this, mSoundArchivePlayerInfoOffset)
        .Get<SoundArchivePlayerInfo>();
}

u32 SoundArchiveFile::WaveSoundInfo::GetChannelPriority() const {
    if (!(mFlags & 1))
        return 64;
    return mChannelPriority;
}

u8 SoundArchiveFile::WaveSoundInfo::GetIsReleasePriorityFix() const {
    if (!(mFlags & 1))
        return 0;
    return mChannelPriority >> 8;
}

u32 SoundArchiveFile::SequenceSoundInfo::GetStartOffset() const {
    if (!(mOptionFlags & 1))
        return 0;
    return mStartOffset;
}

const SoundArchiveFile::InternalFileInfo* SoundArchiveFile::FileInfo::GetInternalFileInfo() const {
    if (mSignature != 0x220c)
        return nullptr;
    return nn::util::ConstBytePtr(this, mFileInfoOffset).Get<InternalFileInfo>();
}

const SoundArchiveFile::ExternalFileInfo* SoundArchiveFile::FileInfo::GetExternalFileInfo() const {
    if (mSignature != 0x220d)
        return nullptr;
    return nn::util::ConstBytePtr(this, mFileInfoOffset).Get<ExternalFileInfo>();
}

// One string-table entry: the string's offset from the table is the third word.
struct StringTableItem {
    u32 _0;
    u32 _4;
    s32 mOffset;
};

struct StringTable {
    StringTableItem items[1];
};

const char* SoundArchiveFile::StringBlockBody::GetString(u32 string_id) const {
    if (string_id == 0xffffffff)
        return nullptr;
    const auto* table =
        static_cast<const StringTable*>(nn::util::ConstBytePtr(this, mStringTableOffset).Get());
    if (!table)
        return nullptr;
    return nn::util::ConstBytePtr(table, table->items[s32(string_id)].mOffset).Get<char>();
}

const SoundArchiveFile::BlockDescriptor* SoundArchiveFile::FileHeader::findBlock(
    u16 signature) const {
    return mBlocks[0].mSignature == signature ? &mBlocks[0]
           : mBlocks[1].mSignature == signature ? &mBlocks[1]
           : mBlocks[2].mSignature == signature ? &mBlocks[2]
                                                : nullptr;
}

u32 SoundArchiveFile::FileHeader::GetStringBlockSize() const {
    return findBlock(0x2000)->mSize;
}

u32 SoundArchiveFile::FileHeader::GetStringBlockOffset() const {
    return findBlock(0x2000)->mOffset;
}

u32 SoundArchiveFile::FileHeader::GetInfoBlockSize() const {
    return findBlock(0x2001)->mSize;
}

u32 SoundArchiveFile::FileHeader::GetInfoBlockOffset() const {
    return findBlock(0x2001)->mOffset;
}

u32 SoundArchiveFile::FileHeader::GetFileBlockOffset() const {
    return findBlock(0x2002)->mOffset;
}

// A reference table: the record count, then one 8-byte item per record whose second word is the
// record's offset from the table. The record lookups below are the same lookup inlined once per
// record type, as in the original.
struct ReferenceTableItem {
    u32 _0;
    s32 offset;
};

struct ReferenceTable {
    u32 count;
    ReferenceTableItem items[1];
};

const SoundArchiveFile::SoundInfo* SoundArchiveFile::InfoBlockBody::GetSoundInfo(u32 sound_id)
    const {
    if (sound_id >> 24 != 1)
        return nullptr;
    const u32 index = sound_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(GetSoundInfoReferenceTable()).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const SoundInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

const SoundArchiveFile::BankInfo* SoundArchiveFile::InfoBlockBody::GetBankInfo(u32 bank_id) const {
    if (bank_id >> 24 != 3)
        return nullptr;
    const u32 index = bank_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(this, mBankInfoTableOffset).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const BankInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

const SoundArchiveFile::PlayerInfo* SoundArchiveFile::InfoBlockBody::GetPlayerInfo(
    u32 player_id) const {
    if (player_id >> 24 != 4)
        return nullptr;
    const u32 index = player_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(GetPlayerInfoReferenceTable()).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const PlayerInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

const SoundArchiveFile::SoundGroupInfo* SoundArchiveFile::InfoBlockBody::GetSoundGroupInfo(
    u32 sound_group_id) const {
    if (sound_group_id >> 24 != 2)
        return nullptr;
    const u32 index = sound_group_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(GetSoundGroupInfoReferenceTable()).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const SoundGroupInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

const SoundArchiveFile::GroupInfo* SoundArchiveFile::InfoBlockBody::GetGroupInfo(u32 group_id)
    const {
    if (group_id >> 24 != 6)
        return nullptr;
    const u32 index = group_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(GetGroupInfoReferenceTable()).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const GroupInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

const SoundArchiveFile::WaveArchiveInfo* SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfo(
    u32 wave_archive_id) const {
    if (wave_archive_id >> 24 != 5)
        return nullptr;
    const u32 index = wave_archive_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(GetWaveArchiveInfoReferenceTable()).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const WaveArchiveInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

const SoundArchiveFile::FileInfo* SoundArchiveFile::InfoBlockBody::GetFileInfo(u32 file_id) const {
    const u32 index = file_id & 0xffffff;
    const auto* table = static_cast<const ReferenceTable*>(
        nn::util::ConstBytePtr(GetFileInfoReferenceTable()).Get());
    if (index >= table->count)
        return nullptr;
    return static_cast<const FileInfo*>(
        nn::util::ConstBytePtr(table, table->items[index].offset).Get());
}

u32 SoundArchiveFile::InfoBlockBody::GetItemFileId(u32 item_id) const {
    switch (item_id >> 24) {
    case 1: {
        const SoundInfo* info = GetSoundInfo(item_id);
        if (!info)
            return -1;
        return info->mItemFileId;
    }
    case 2: {
        const SoundGroupInfo* sound_group = GetSoundGroupInfo(item_id);
        if (!sound_group)
            return -1;
        const SoundInfo* info = GetSoundInfo(sound_group->mSoundId);
        if (!info)
            return -1;
        return info->mItemFileId;
    }
    case 3: {
        const BankInfo* info = GetBankInfo(item_id);
        if (!info)
            return -1;
        return info->mItemFileId;
    }
    case 5: {
        const WaveArchiveInfo* info = GetWaveArchiveInfo(item_id);
        if (!info)
            return -1;
        return info->mItemFileId;
    }
    case 6: {
        const GroupInfo* info = GetGroupInfo(item_id);
        if (!info)
            return -1;
        return info->mItemFileId;
    }
    default:
        return -1;
    }
}

u32 SoundArchiveFile::InfoBlockBody::GetItemPrefetchFileId(u32 item_id) const {
    const SoundInfo* info = GetSoundInfo(item_id);
    if (!info)
        return -1;
    // Only wave sounds carry a prefetch file id (signature 0x2201).
    if (info->GetSignature() != 0x2201)
        return -1;
    return info->GetWaveSoundInfo()->GetPrefetchFileId();
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

u32 SoundArchiveFile::PlayerInfo::GetPlayerHeapSize() const {
    const u32* option = &mOptionFlags;
    if (!(*option & 2))
        return 0;
    return option[(*option & 1) + 1];
}

u32 SoundArchiveFile::WaveArchiveInfo::GetWaveCount() const {
    const u32* option = &mOptionFlags;
    if (!(*option & 2))
        return 0;
    return option[(*option & 1) + 1];
}
u32 SoundArchiveFile::SoundInfo::GetPanMode() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return 0;
    return static_cast<u8>(value);
}

u32 SoundArchiveFile::SoundInfo::GetPanCurve() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return 0;
    return static_cast<u8>(value >> 8);
}

u32 SoundArchiveFile::SoundInfo::GetPlayerPriority() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 64;
    return value;
}

u8 SoundArchiveFile::SoundInfo::GetActorPlayerId() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0;
    return value >> 8;
}

bool SoundArchiveFile::SoundInfo::IsFrontBypass() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 17, &value))
        return false;
    return value & 1;
}

void SoundArchiveFile::SequenceSoundInfo::GetBankIds(u32* bankIds) const {
    const u32* table = nn::util::ConstBytePtr(this, mBankIdTableOffset).Get<u32>();
    for (u32 i = 0; i < 4; ++i)
        bankIds[i] = i < table[0] ? table[i + 1] : 0xffffffff;
}

u32 SoundArchiveFile::SequenceSoundInfo::GetChannelPriority() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return 64;
    return value;
}

bool SoundArchiveFile::SequenceSoundInfo::IsReleasePriorityFix() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return true;
    return (value >> 8) & 0xff;
}

}  // namespace detail
}  // namespace atk
}  // namespace nn
