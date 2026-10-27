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

}  // namespace nn::atk::detail
