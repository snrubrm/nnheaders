#include <nn/atk/detail/driver/SequenceSoundLoader.h>
#include <nn/atk/detail/TaskManager.h>

namespace nn::atk::detail::driver {

void SequenceSoundLoader::Finalize() {
    mFreePlayerHeapTask.mPlayerHeap = mDataLoadTask.mPlayerHeap;
    TaskManager::GetInstance().AppendTask(&mFreePlayerHeapTask,
                                          static_cast<TaskManager::TaskPriority>(1));
}

bool SequenceSoundLoader::IsInUse() {
    return !mDataLoadTask.IsFinishedMaybe() || !mFreePlayerHeapTask.IsFinishedMaybe();
}

}  // namespace nn::atk::detail::driver
