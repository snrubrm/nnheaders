#pragma once

#include <nn/atk/detail/BankFile.h>

namespace nn::atk::detail {

class BankFileReader {
public:
    explicit BankFileReader(const void* file);
    void Initialize(const void* file);
    void Finalize();
    const u32* GetWaveIdTable() const;

private:
    const BankFile::FileHeader* mFile;
    const BankFile::InfoBlockBody* mInfo;
    bool mInitialized;
};
static_assert(sizeof(BankFileReader) == 0x18);

}  // namespace nn::atk::detail
