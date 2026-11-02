#include <nn/atk/detail/DriverCommand.h>
#include <nn/atk/detail/driver/SoundThread.h>

namespace nn::atk::detail {

void DriverCommand::RequestProcessCommand() {
    driver::SoundThread::GetInstance().ForceWakeup();
}

}  // namespace nn::atk::detail
