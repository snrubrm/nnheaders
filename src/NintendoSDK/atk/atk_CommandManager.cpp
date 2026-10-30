#include <nn/atk/detail/CommandManager.h>

namespace nn::atk::detail {

CommandManager::CommandManager()
    : _0(0), mProcessCommand(nullptr), mRequestProcessCommand(nullptr), _160(0), _2b8(0),
      _2e0(nullptr), _2e8(0), _2f0(0), _2f8(0), _300(1) {}

CommandManager::~CommandManager() {
    Finalize();
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

}  // namespace nn::atk::detail
