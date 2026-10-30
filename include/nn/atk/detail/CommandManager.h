#pragma once

#include <nn/os/os_MessageQueue.h>
#include <nn/types.h>
#include <atomic>

namespace nn::atk::detail {

// FlushCommand allocates this 0x18-byte command header, and the driver dispatches
// the linked commands by the value at +0x8.
struct Command {
    Command* next;
    u32 _8;
    u32 _c;
    u64 _10;
};
static_assert(sizeof(Command) == 0x18);

// Partial native model: both queues, their buffers, and the observed state are represented.
class CommandManager {
public:
    CommandManager();
    ~CommandManager();
    // Native DriverCommand caller discards the result; void is inferred.
    void Initialize(void* memory, size_t size, void (*process)(Command*));
    void Finalize();
    // Native returns the current 32-bit sequence; unsigned return is inferred.
    u32 PushCommand(Command* command);
    bool IsFinishCommand(u32 sequence) const;

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
    u32 _2d8;
    u8 _2dc[4];
    void* _2e0;
    u64 _2e8;
    u64 _2f0;
    u64 _2f8;
    u8 _300;
};

}  // namespace nn::atk::detail
