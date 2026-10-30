#pragma once

#include <nn/atk/TaskProfileLogger.h>
#include <nn/types.h>

namespace nn::atk::detail {

// Partial native model: the singleton constructs the logger at +0x1a8.
class TaskManager {
public:
    TaskManager();
    ~TaskManager();

    // Native sound-system callers discard both results; void is inferred.
    void Initialize(bool profilingEnabled);
    void Finalize();

private:
    u8 _0[0x1a8];
    TaskProfileLogger mProfileLogger;
};

}  // namespace nn::atk::detail
