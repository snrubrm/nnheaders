#include <nn/atk/detail/WaveSoundFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

const u32* WaveSoundFile::InfoBlockBody::GetWaveIdTable() const {
    return nn::util::ConstBytePtr(this, mWaveIdTableOffset).Get<u32>();
}

}  // namespace nn::atk::detail
