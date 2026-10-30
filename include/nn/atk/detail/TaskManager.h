#pragma once

#include <nn/atk/TaskProfileLogger.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/types.h>

namespace nn::atk::detail {

class Task;

// Partial native model: the singleton constructs the logger at +0x1a8.
class TaskManager {
public:
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

private:
    Task* PopTask();
    Task* PeekTask();

    u8 _0[0x30];
    Task* mCurrentTask;  // +0x30
    u8 _38;
    u8 _39[0x60 - 0x39];
    nn::os::MessageQueueType mQueue;  // +0x60
    u64 mQueueBuffer[32];             // +0xa8
    TaskProfileLogger mProfileLogger;
};

}  // namespace nn::atk::detail
