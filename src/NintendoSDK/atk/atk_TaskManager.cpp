#include <nn/atk/detail/TaskManager.h>

namespace nn::atk::detail {

void TaskManager::Initialize(bool profilingEnabled) {
    mProfileLogger.SetProfilingEnabled(profilingEnabled);
}

void TaskManager::Finalize() {
    mProfileLogger.SetProfilingEnabled(false);
    mProfileLogger.Finalize();
}

}  // namespace nn::atk::detail
