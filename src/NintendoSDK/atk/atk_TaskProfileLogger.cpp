#include <nn/atk/TaskProfileLogger.h>

namespace nn::atk {

TaskProfileLogger::TaskProfileLogger() {
    os::InitializeMutex(&mMutex, true, 0);
    mProfilingEnabled = false;
}

void TaskProfileLogger::SetProfilingEnabled(bool enabled) {
    mProfilingEnabled = enabled;
}

void TaskProfileLogger::Finalize() {
    os::LockMutex(&mMutex);
    mReaders.clear();
    os::UnlockMutex(&mMutex);
}

}  // namespace nn::atk
