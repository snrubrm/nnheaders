#pragma once

#include <nn/atk/TaskProfileLogger.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/os/os_Mutex.h>
#include <nn/types.h>

namespace nn::atk::detail {

class Task;

// Partial native model: the singleton constructs the logger at +0x1a8.
class TaskManager {
public:
    // Native indexes three roots using a zero-extended 32-bit priority.
    // The unsigned underlying type is inferred; enumerator names are unresolved.
    enum TaskPriority : u32 {};

    TaskManager();
    ~TaskManager();

    // Native sound-system callers discard both results; void is inferred.
    void Initialize(bool profilingEnabled);
    void Finalize();
    // TaskThread::Run discards the native result; void is inferred.
    void ExecuteTask();
    // TaskThread callers discard the native results; void is inferred.
    void WaitTask();
    void CancelWaitTask();
    // Native callers discard the result; void is inferred.
    void AppendTask(Task* task, TaskPriority priority);

private:
    Task* PopTask();
    Task* PeekTask();

    nn::util::detail::IntrusiveListImplementation mTaskLists[3];  // +0x0
    Task* mCurrentTask;  // +0x30
    u8 _38;
    u8 _39[7];
    nn::os::MutexType mMutex;         // +0x40
    nn::os::MessageQueueType mQueue;  // +0x60
    u64 mQueueBuffer[32];             // +0xa8
    TaskProfileLogger mProfileLogger;
};

}  // namespace nn::atk::detail
