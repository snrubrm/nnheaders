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

const void* SoundArchiveLoader::GetFileAddressFromSoundArchive(u32 file_id) const {
    if (!mArchive)
        return nullptr;
    return mArchive->detail_GetFileAddress(file_id);
}

}  // namespace nn::atk::detail
