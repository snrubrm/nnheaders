#include <nn/atk/detail/SoundArchiveManager.h>

namespace nn::atk::detail {

SoundArchiveManager::SoundArchiveManager()
    : mSoundArchive(nullptr), mSoundDataManager(nullptr), mTargetSoundArchive(nullptr),
      mTargetSoundDataManager(nullptr) {}

void SoundArchiveManager::Initialize(const nn::atk::SoundArchive* archive,
                                     const nn::atk::SoundDataManager* dataManager) {
    mSoundArchive = archive;
    mSoundDataManager = dataManager;
    mAddons.clear();
    mTargetSoundArchive = mSoundArchive;
    mTargetSoundDataManager = mSoundDataManager;
}

void SoundArchiveManager::Finalize() {
    mTargetSoundArchive = nullptr;
    mTargetSoundDataManager = nullptr;
    mAddons.clear();
    mSoundArchive = nullptr;
    mSoundDataManager = nullptr;
}


SoundArchiveManager::~SoundArchiveManager() {
}

}  // namespace nn::atk::detail
