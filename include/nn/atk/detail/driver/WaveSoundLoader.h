#pragma once

#include <nn/atk/detail/Task.h>

namespace nn::atk::detail::driver {

// WaveSoundRuntime::Initialize 0x710135aec0 constructs Task subobjects at +0 and +0xa0.
class WaveSoundLoader {
public:
    bool IsInUse();

private:
    class DataLoadTask : public Task {
    public:
        ~DataLoadTask() override;
        void Execute(TaskProfileLogger& logger) override;

    private:
        u8 _48[0xa0 - 0x48];
    };

    class FreePlayerHeapTask : public Task {
    public:
        ~FreePlayerHeapTask() override;
        void Execute(TaskProfileLogger& logger) override;

    private:
        // Execute reads the last pointer at +0x80; the heap manager follows at loader +0x128.
        u8 _48[0x88 - 0x48];
    };
    static_assert(sizeof(DataLoadTask) == 0xa0);
    static_assert(sizeof(FreePlayerHeapTask) == 0x88);

    DataLoadTask mDataLoadTask;
    FreePlayerHeapTask mFreePlayerHeapTask;
};

}  // namespace nn::atk::detail::driver
