#include <nn/atk/detail/WaveSoundFile.h>
#include <nn/atk/detail/Util.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {


const u32* WaveSoundFile::InfoBlockBody::GetWaveIdTable() const {
    return nn::util::ConstBytePtr(this, mWaveIdTableOffset).Get<u32>();
}

const WaveSoundFile::WaveSoundInfo* WaveSoundFile::WaveSoundData::GetWaveSoundInfo() const {
    return nn::util::ConstBytePtr(this, mWaveSoundInfo.mOffset).Get<WaveSoundInfo>();
}

const WaveSoundFile::NoteInfo* WaveSoundFile::WaveSoundData::GetNoteInfo(u32 index) const {
    const ReferenceTable* table =
        nn::util::ConstBytePtr(this, mNoteInfoTable.mOffset).Get<ReferenceTable>();
    if (index >= table->mCount)
        return nullptr;
    if (table->mItems[index].mTypeId != 0x4902)
        return nullptr;
    return nn::util::ConstBytePtr(table, table->mItems[index].mOffset).Get<NoteInfo>();
}

u32 WaveSoundFile::NoteInfo::GetOriginalKey() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 0, &value))
        return 0x40;
    return value;
}

u32 WaveSoundFile::NoteInfo::GetVolume() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return 0x60;
    return value;
}

u8 WaveSoundFile::NoteInfo::GetPan() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0x40;
    return value;
}

u8 WaveSoundFile::NoteInfo::GetSurroundPan() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0;
    return value >> 8;
}

f32 WaveSoundFile::NoteInfo::GetPitch() const {
    f32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 3, &value))
        return 1.0f;
    return value;
}

const WaveSoundFile::InfoBlock* WaveSoundFile::FileHeader::GetInfoBlock() const {
    return static_cast<const InfoBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x6800));
}

const WaveSoundFile::WaveSoundData* WaveSoundFile::InfoBlockBody::GetWaveSoundData(u32 index) const {
    const Util::ReferenceTable* table =
        nn::util::ConstBytePtr(this, mWaveSoundDataReferenceTableOffset).Get<Util::ReferenceTable>();
    return static_cast<const WaveSoundData*>(table->GetReferedItem(index, 0x4900));
}

u8 WaveSoundFile::WaveSoundInfo::GetPan() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 0, &value))
        return 0x40;
    return value;
}

u8 WaveSoundFile::WaveSoundInfo::GetSurroundPan() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 0, &value))
        return 0;
    return value >> 8;
}

f32 WaveSoundFile::WaveSoundInfo::GetPitch() const {
    f32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return 1.0f;
    return value;
}

u8 WaveSoundFile::WaveSoundInfo::GetLpfFreq() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0x40;
    return value;
}

u8 WaveSoundFile::WaveSoundInfo::GetBiquadType() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0;
    return value >> 8;
}

u16 WaveSoundFile::WaveSoundInfo::GetBiquadValue() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0;
    return value >> 16;
}

}  // namespace nn::atk::detail
