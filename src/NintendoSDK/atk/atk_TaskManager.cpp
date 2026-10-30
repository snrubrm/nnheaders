#include <nn/atk/detail/TaskManager.h>
#include <nn/atk/detail/Task.h>

namespace nn::atk::detail {

void TaskManager::Initialize(bool profilingEnabled) {
    mProfileLogger.SetProfilingEnabled(profilingEnabled);
}

void TaskManager::Finalize() {
    mProfileLogger.SetProfilingEnabled(false);
    mProfileLogger.Finalize();
}

void TaskManager::ExecuteTask() {
    while (Task* task = PopTask()) {
        mCurrentTask = task;
        task->Execute(mProfileLogger);
        task->m_Status = 3;
        task->m_Event.Signal();
        mCurrentTask = nullptr;
    }
}

}  // namespace nn::atk::detail
