#include <nn/atk/detail/CommandManager.h>

namespace nn::atk::detail {

CommandManager::CommandManager()
    : _0(0), mProcessCommand(nullptr), mRequestProcessCommand(nullptr), _160(0), _2b8(0),
      _2e0(nullptr), _2e8(0), _2f0(0), _2f8(0), _300(1) {}

CommandManager::~CommandManager() {
    Finalize();
}

void CommandManager::Initialize(void* memory, size_t size, void (*process)(Command*)) {
    _2e8 = size / sizeof(u32);
    mProcessCommand = process;
    mRequestProcessCommand = nullptr;
    _300 = 0;
    _2f8 = 0;
    _2f0 = 0;
    _2e0 = memory;
    _2d4 = 0;
    _2c8 = nullptr;
    _2c0 = nullptr;
    if (!_160) {
        nn::os::InitializeMessageQueue(&mCommandQueue, mCommandBuffer, 32);
        _160 = 1;
    }
    if (!_2b8) {
        nn::os::InitializeMessageQueue(&mReplyQueue, mReplyBuffer, 33);
        _2b8 = 1;
    }
    _2d0.store(0, std::memory_order_release);
    _0 = 1;
}

void CommandManager::Finalize() {
    if (_160) {
        nn::os::FinalizeMessageQueue(&mCommandQueue);
        _160 = 0;
    }
    if (_2b8) {
        nn::os::FinalizeMessageQueue(&mReplyQueue);
        _2b8 = 0;
    }
    _0 = 0;
}

u32 CommandManager::PushCommand(Command* command) {
    if (_2c8)
        _2c8->next = command;
    else
        _2c0 = command;
    _2c8 = command;
    command->next = nullptr;
    return _2d4;
}

bool CommandManager::IsFinishCommand(u32 sequence) const {
    constexpr u32 wrapWindow = 1u << 30;
    if (_2d8 < sequence)
        return sequence - _2d8 >= wrapWindow;
    return _2d8 - sequence < wrapWindow;
}

}  // namespace nn::atk::detail
