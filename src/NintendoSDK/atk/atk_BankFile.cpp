#include <nn/atk/detail/BankFile.h>
#include <nn/atk/detail/Util.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {


const u32* BankFile::InfoBlockBody::GetWaveIdTable() const {
    return nn::util::ConstBytePtr(this, mWaveIdTableOffset).Get<u32>();
}

const u32* BankFile::InfoBlockBody::GetInstrumentReferenceTable() const {
    return nn::util::ConstBytePtr(this, mInstrumentReferenceTableOffset).Get<u32>();
}

u32 BankFile::VelocityRegion::GetOriginalKey() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 0, &value))
        return 0x3c;
    return value;
}

u32 BankFile::VelocityRegion::GetVolume() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 1, &value))
        return 0x7f;
    return value;
}

u8 BankFile::VelocityRegion::GetPan() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 2, &value))
        return 0x40;
    return value;
}

f32 BankFile::VelocityRegion::GetPitch() const {
    f32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 3, &value))
        return 1.0f;
    return value;
}

bool BankFile::VelocityRegion::IsIgnoreNoteOff() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 4, &value))
        return false;
    return (value & 0xff) != 0;
}

u8 BankFile::VelocityRegion::GetKeyGroup() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 4, &value))
        return 0;
    return value >> 8;
}

u16 BankFile::VelocityRegion::GetInterpolationType() const {
    u32 value;
    if (!Util::ReadOptionParameter(&mOptionFlags, 4, &value))
        return 0;
    return value >> 16;
}

const BankFile::InfoBlock* BankFile::FileHeader::GetInfoBlock() const {
    return static_cast<const InfoBlock*>(Util::FindBlock(this, mBlockCount, mBlocks, 0x5800));
}

const BankFile::Instrument* BankFile::InfoBlockBody::GetInstrument(int index) const {
    const Util::ReferenceTable* table = nn::util::ConstBytePtr(this, mInstrumentReferenceTableOffset)
                                            .Get<Util::ReferenceTable>();
    if (table->item[index].typeId != 0x5900)
        return nullptr;
    return static_cast<const Instrument*>(table->GetReferedItem(index));
}

}  // namespace nn::atk::detail
