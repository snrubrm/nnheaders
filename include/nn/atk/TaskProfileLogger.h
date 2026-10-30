#pragma once

#include <nn/os/os_Mutex.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {

class TaskProfileLogger {
public:
    TaskProfileLogger();
    void SetProfilingEnabled(bool enabled);
    void Finalize();

private:
    util::detail::IntrusiveListImplementation mReaders;
    os::MutexType mMutex;
    bool mProfilingEnabled;
};
static_assert(sizeof(TaskProfileLogger) == 0x38);

}  // namespace nn::atk
