#include <nn/atk/detail/BankFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

const u32* BankFile::InfoBlockBody::GetWaveIdTable() const {
    return nn::util::ConstBytePtr(this, mWaveIdTableOffset).Get<u32>();
}

const u32* BankFile::InfoBlockBody::GetInstrumentReferenceTable() const {
    return nn::util::ConstBytePtr(this, mInstrumentReferenceTableOffset).Get<u32>();
}

}  // namespace nn::atk::detail
