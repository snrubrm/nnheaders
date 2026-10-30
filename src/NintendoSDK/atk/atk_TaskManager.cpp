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

void TaskManager::WaitTask() {
    _38 = 0;
    while (!PeekTask() && !_38) {
        u64 message;
        nn::os::ReceiveMessageQueue(&message, &mQueue);
        if (message == 0)
            break;
    }
}

void TaskManager::CancelWaitTask() {
    _38 = 1;
    nn::os::SendMessageQueue(&mQueue, 0);
}

}  // namespace nn::atk::detail
