#include <nn/atk/detail/driver/WaveSoundLoader.h>
#include <nn/atk/detail/TaskManager.h>

namespace nn::atk::detail::driver {

void WaveSoundLoader::Finalize() {
    mFreePlayerHeapTask.mPlayerHeap = mDataLoadTask.mPlayerHeap;
    TaskManager::GetInstance().AppendTask(&mFreePlayerHeapTask,
                                          static_cast<TaskManager::TaskPriority>(1));
}

bool WaveSoundLoader::IsInUse() {
    return !mDataLoadTask.IsFinishedMaybe() || !mFreePlayerHeapTask.IsFinishedMaybe();
}

}  // namespace nn::atk::detail::driver
