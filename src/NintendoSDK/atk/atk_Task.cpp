#include <nn/atk/detail/Task.h>

namespace nn::atk::detail {

Task::Task() : m_Event(os::EventClearMode_ManualClear), m_Status(0), m_Id(0) {
    m_Event.Signal();
}

Task::~Task() {}

}  // namespace nn::atk::detail
