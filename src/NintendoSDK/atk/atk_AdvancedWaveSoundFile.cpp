#include <nn/atk/detail/AdvancedWaveSoundFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

const u32* AdvancedWaveSoundFile::InfoBlockBody::GetTrackReferenceTable() const {
    return nn::util::ConstBytePtr(this, mTrackReferenceTableOffset).Get<u32>();
}

}  // namespace nn::atk::detail
