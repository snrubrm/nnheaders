#pragma once

#include <nn/os.h>
#include <nn/types.h>

namespace nn::atk::detail::fnd {

// The sound system's worker thread. The status word at +0 is the state Release waits for
// (1 while the body runs, 2 when it has exited); the os thread object follows at +8.
class Thread {
public:
    class Handler {
    public:
        virtual ~Handler();
        // Native ThreadMain ignores the result; void is inferred. Purity is unresolved.
        virtual void Run(void* arg);
    };

    enum AffinityMask : u32 {};

    struct RunArgs {
        RunArgs();

        const char* name;
        void* stack;
        size_t stackSize;
        s32 core;
        AffinityMask affinity;
        s32 priority;
        void* argument;
        Handler* handler;
    };

    Thread();
    // 0x710135d698
    ~Thread();
    bool Run(const RunArgs& args);
    // 0x710135d780 / 0x710135d78c: the state the thread body runs in.
    void OnRun();
    void OnExit();
    // 0x710135d89c / 0x710135d860: wait for or destroy the os thread.
    void Join();
    void Detach();
    // 0x710135d874 / 0x710135d88c
    void SetName(const char* name);
    void SetAffinityMask(int core, AffinityMask mask);
    // 0x710135d748: detaches a thread whose body has exited and marks it released.
    void Release();
    // 0x710135d898
    void Resume();

private:
    // Native Create returns 0 or 1; bool is inferred from its callers.
    bool Create(nn::os::ThreadType& thread, s64& handle, const RunArgs& args);

    /* 0x000 */ s32 mStatus = 0;
    u8 _4[4];
    /* 0x008 */ nn::os::ThreadType mThread;
    /* 0x1c8 */ s64 _1c8 = 0xffffffffLL;
    /* 0x1d0 */ u32 _1d0 = 0x10;
    u8 _1d4[4];
    /* 0x1d8 */ void* mArgument;
    /* 0x1e0 */ Handler* mHandler = nullptr;
    /* 0x1e8 */ bool _1e8 = false;
};
static_assert(sizeof(Thread::RunArgs) == 0x38);

}  // namespace nn::atk::detail::fnd
