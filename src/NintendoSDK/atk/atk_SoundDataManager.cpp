#include <nn/atk/SoundDataManager.h>

namespace nn::atk {

const void* SoundDataManager::detail_GetFileAddress(u32 file_id) const {
    return GetFileAddressImpl(file_id);
}

}  // namespace nn::atk
