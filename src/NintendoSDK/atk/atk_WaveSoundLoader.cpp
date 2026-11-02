#include <nn/atk/detail/driver/WaveSoundLoader.h>
#include <nn/atk/SoundPlayer.h>
#include <nn/atk/detail/TaskManager.h>

namespace nn::atk::detail::driver {

bool WaveSoundLoader::TryWait() {
    if (!mDataLoadTask.mPlayerHeap) {
        mDataLoadTask.mPlayerHeap = mDataLoadTask.mSoundPlayer->detail_AllocPlayerHeap();
        if (!mDataLoadTask.mPlayerHeap)
            return false;
    }
    const u32 status = mDataLoadTask.m_Status;
    if (status == 3 || status == 4)
        return true;
    if (status == 0) {
        TaskManager::GetInstance().AppendTask(&mDataLoadTask,
                                              static_cast<TaskManager::TaskPriority>(1));
    }
    return false;
}

void WaveSoundLoader::Finalize() {
    mFreePlayerHeapTask.mPlayerHeap = mDataLoadTask.mPlayerHeap;
    TaskManager::GetInstance().AppendTask(&mFreePlayerHeapTask,
                                          static_cast<TaskManager::TaskPriority>(1));
}

bool WaveSoundLoader::IsInUse() {
    return !mDataLoadTask.IsFinishedMaybe() || !mFreePlayerHeapTask.IsFinishedMaybe();
}

}  // namespace nn::atk::detail::driver
