#include <nn/atk/detail/SequenceSoundFileReader.h>

namespace nn::atk::detail {

const void* SequenceSoundFileReader::GetSequenceData() const {
    return mSequenceData;
}

}  // namespace nn::atk::detail
