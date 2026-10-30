#pragma once

#include <nn/os/os_MessageQueue.h>
#include <nn/types.h>
#include <atomic>

namespace nn::atk::detail {

class Command;

// Partial native model: both queues, their buffers, and the observed state are represented.
class CommandManager {
public:
    CommandManager();
    ~CommandManager();
    // Native DriverCommand caller discards the result; void is inferred.
    void Initialize(void* memory, size_t size, void (*process)(Command*));
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
    u8 _2b9[7];
    Command* _2c0;
    Command* _2c8;
    std::atomic<u32> _2d0;
    u32 _2d4;
    u8 _2d8[8];
    void* _2e0;
    u64 _2e8;
    u64 _2f0;
    u64 _2f8;
    u8 _300;
};

}  // namespace nn::atk::detail
