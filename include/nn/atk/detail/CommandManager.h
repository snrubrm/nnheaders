#pragma once

#include <nn/os/os_MessageQueue.h>
#include <nn/types.h>

namespace nn::atk::detail {

class Command;

// Partial native model: both queues, their buffers, and the observed state are represented.
class CommandManager {
public:
    CommandManager();
    ~CommandManager();
    void Finalize();

private:
    u8 _0;
    u8 _1[7];
    void (*mProcessCommand)(Command*);
    void (*mRequestProcessCommand)();
    nn::os::MessageQueueType mCommandQueue;  // +0x18
    u64 mCommandBuffer[32];                  // +0x60
    u8 _160;
    u8 _161[7];
    nn::os::MessageQueueType mReplyQueue;     // +0x168
    u64 mReplyBuffer[33];                    // +0x1b0
    u8 _2b8;
    u8 _2b9[0x2e0 - 0x2b9];
    void* _2e0;
    u64 _2e8;
    u64 _2f0;
    u64 _2f8;
    u8 _300;
};

}  // namespace nn::atk::detail
