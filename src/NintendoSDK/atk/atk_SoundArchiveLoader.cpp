#include <nn/atk/SoundArchive.h>
#include <nn/atk/SoundDataManager.h>

namespace nn::atk::detail {

SoundArchiveLoader::SoundArchiveLoader() : mArchive(nullptr) {}

SoundArchiveLoader::~SoundArchiveLoader() {
    mArchive = nullptr;
}

void SoundArchiveLoader::SetSoundArchive(const SoundArchive* archive) {
    mArchive = archive;
}

bool SoundArchiveLoader::IsAvailable() const {
    if (!mArchive)
        return false;
    return mArchive->IsAvailable();
}

bool SoundArchiveLoader::LoadData(const char* label, SoundMemoryAllocatable* allocator,
                                  u32 load_flag, size_t unknown) {
    return LoadData(mArchive->GetItemId(label), allocator, load_flag, unknown);
}

const void* SoundArchiveLoader::GetFileAddressFromSoundArchive(u32 file_id) const {
    if (!mArchive)
        return nullptr;
    return mArchive->detail_GetFileAddress(file_id);
}

}  // namespace nn::atk::detail
