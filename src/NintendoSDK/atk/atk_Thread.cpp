#include <nn/atk/detail/fnd/Thread.h>

namespace nn::atk::detail::fnd {

Thread::RunArgs::RunArgs()
    : name(""), stack(nullptr), stackSize(0), core(-1), affinity{}, priority(16),
      argument(nullptr), handler(nullptr) {}

// 0x710135d798
Thread::Thread() = default;

// 0x710135d698
Thread::~Thread() {}

// NON_MATCHING: OnRun and OnExit inline; the compiler also loads the callback
// fields earlier and saves an additional register.
void Thread::ThreadMain::Run(void* arg) {
    auto* thread = static_cast<Thread*>(arg);
    thread->_1e8 = false;
    thread->OnRun();
    thread->mHandler->Run(thread->mArgument);
    thread->OnExit();
    thread->_1e8 = true;
}

// NON_MATCHING: SetName and SetAffinityMask inline, and the empty Resume call is
// eliminated; native keeps all three calls and uses a smaller stack frame.
bool Thread::Run(const RunArgs& args) {
    if (!args.stack || !args.stackSize || args.priority > 31 || !args.handler)
        return false;

    mArgument = args.argument;
    mHandler = args.handler;
    if (!Create(mThread, _1c8, args))
        return false;

    SetName(args.name);
    if (args.affinity != 0)
        SetAffinityMask(args.core, args.affinity);
    _1d0 = args.priority;
    Resume();
    return true;
}

// 0x710135d780
void Thread::OnRun() {
    mStatus = 1;
}

// 0x710135d78c
void Thread::OnExit() {
    mStatus = 2;
}

// 0x710135d89c
void Thread::Join() {
    nn::os::WaitThread(&mThread);
}

// 0x710135d860
void Thread::Detach() {
    if (_1e8)
        nn::os::DestroyThread(&mThread);
}

// 0x710135d874
void Thread::SetName(const char* name) {
    nn::os::SetThreadNamePointer(&mThread, name == nullptr ? "" : name);
}

// 0x710135d88c
void Thread::SetAffinityMask(int core, AffinityMask mask) {
    nn::os::SetThreadCoreMask(&mThread, core, mask);
}

// NON_MATCHING: clang inlines Detach() into this function; the original keeps the out-of-line
// call and is two instructions shorter.
// 0x710135d748
void Thread::Release() {
    if (mStatus == 2) {
        Detach();
        mStatus = 3;
    }
}

// 0x710135d898
void Thread::Resume() {}

}  // namespace nn::atk::detail::fnd
