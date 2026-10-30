#include <nn/atk/SoundDataManager.h>

namespace nn::atk {

size_t SoundDataManager::GetRequiredMemSize(const SoundArchive* archive) const {
    size_t file_count = archive->detail_GetFileCount();
    return file_count * 16 + 4;
}

const void* SoundDataManager::detail_GetFileAddress(u32 file_id) const {
    return GetFileAddressImpl(file_id);
}

}  // namespace nn::atk
