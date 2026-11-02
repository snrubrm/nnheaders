#include <nn/atk/detail/driver/SequenceSoundLoader.h>
#include <nn/atk/detail/TaskManager.h>

namespace nn::atk::detail::driver {

// NON_MATCHING: the compiler schedules the paired pointer stores in a different order.
SequenceSoundLoader::LoadInfo::LoadInfo(const SoundArchive* archive,
                                       const SoundDataManager* dataManager,
                                       LoadItemInfo* sequence, LoadItemInfo* banks,
                                       SoundPlayer* player)
    : mArchive(archive), mDataManager(dataManager), mSequence(sequence),
      mBanks{banks, &banks[1], &banks[2], &banks[3]}, mSoundPlayer(player) {}

void SequenceSoundLoader::Finalize() {
    mFreePlayerHeapTask.mPlayerHeap = mDataLoadTask.mPlayerHeap;
    TaskManager::GetInstance().AppendTask(&mFreePlayerHeapTask,
                                          static_cast<TaskManager::TaskPriority>(1));
}

bool SequenceSoundLoader::IsInUse() {
    return !mDataLoadTask.IsFinishedMaybe() || !mFreePlayerHeapTask.IsFinishedMaybe();
}

}  // namespace nn::atk::detail::driver
