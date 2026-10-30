#include <nn/atk/detail/SoundArchiveManager.h>
#include <nn/atk/SoundArchive.h>

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

void SoundArchiveManager::ChangeTargetArchive(const char* name) {
    mTargetSoundArchive = mSoundArchive;
    mTargetSoundDataManager = mSoundDataManager;
    if (!name)
        return;
    for (auto& addon : mAddons) {
        if (addon.IsSameName(name)) {
            mTargetSoundArchive = addon.mSoundArchive;
            mTargetSoundDataManager = addon.mSoundDataManager;
            return;
        }
    }
}

// NON_MATCHING: availability aggregation uses different register and branch layout.
bool SoundArchiveManager::IsAvailable() const {
    if (!mSoundArchive)
        return false;
    bool available = mSoundArchive->IsAvailable();
    for (const auto& addon : mAddons)
        available = addon.mSoundArchive->IsAvailable() && available;
    return available;
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
