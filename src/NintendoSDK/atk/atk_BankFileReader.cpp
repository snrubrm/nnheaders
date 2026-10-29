#include <nn/atk/detail/BankFileReader.h>

namespace nn::atk::detail {

BankFileReader::BankFileReader(const void* file)
    : mFile(nullptr), mInfo(nullptr), mInitialized(false) {
    Initialize(file);
}

void BankFileReader::Initialize(const void* file) {
    const auto* header = static_cast<const BankFile::FileHeader*>(file);
    if (!header || header->mSignature != 0x4b4e4246 || header->mVersion != 0x10000)
        return;
    mFile = header;
    const auto* block = header->GetInfoBlock();
    if (block->mSignature == 0x4f464e49) {
        mInfo = &block->mBody;
        mInitialized = true;
    }
}

void BankFileReader::Finalize() {
    if (mInitialized) {
        mInitialized = false;
        mFile = nullptr;
        mInfo = nullptr;
    }
}

// NON_MATCHING: the compiler emits a tail call where the original returns after the call.
const u32* BankFileReader::GetWaveIdTable() const {
    return mInitialized ? mInfo->GetWaveIdTable() : nullptr;
}

}  // namespace nn::atk::detail
