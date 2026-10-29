#pragma once

#include <nn/os/os_Event.h>
#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {
class TaskProfileLogger;
}

namespace nn::atk::detail {

// A unit of work for the sound task manager. Layout from the constructor (0x7101353040) and
// TaskManager::ExecuteTask / RemoveTaskById: the manager links tasks through the node at +0x8,
// waits on the event at +0x18, sets the status at +0x40 (3 when executed, 4 when removed) and
// matches the id at +0x44.
class Task {
public:
    Task();
    virtual ~Task();
    virtual void Execute(TaskProfileLogger& logger) = 0;

private:
    util::IntrusiveListNode m_TaskLink;  // +0x8
    os::Event m_Event;                   // +0x18
    u32 m_Status;                        // +0x40
    u32 m_Id;                            // +0x44
};
static_assert(sizeof(Task) == 0x48);

}  // namespace nn::atk::detail
