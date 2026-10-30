#pragma once

#include <nn/atk/TaskProfileLogger.h>
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

private:
    Task* PopTask();

    u8 _0[0x30];
    Task* mCurrentTask;  // +0x30
    u8 _38[0x1a8 - 0x38];
    TaskProfileLogger mProfileLogger;
};

}  // namespace nn::atk::detail
