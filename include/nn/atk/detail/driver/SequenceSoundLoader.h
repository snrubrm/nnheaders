#pragma once

#include <nn/atk/detail/Task.h>

namespace nn::atk::detail::driver {

// LoaderManager::Create 0x7101349754 constructs the two Task subobjects at +0 and +0x118.
class SequenceSoundLoader {
public:
    bool IsInUse();

private:
    class DataLoadTask : public Task {
    public:
        ~DataLoadTask() override;
        void Execute(TaskProfileLogger& logger) override;

    private:
        u8 _48[0x118 - 0x48];
    };

    class FreePlayerHeapTask : public Task {
    public:
        ~FreePlayerHeapTask() override;
        void Execute(TaskProfileLogger& logger) override;

    private:
        // Execute reads the last pointer at +0xb8; the heap manager follows at loader +0x1d8.
        u8 _48[0xc0 - 0x48];
    };
    static_assert(sizeof(DataLoadTask) == 0x118);
    static_assert(sizeof(FreePlayerHeapTask) == 0xc0);

    DataLoadTask mDataLoadTask;
    FreePlayerHeapTask mFreePlayerHeapTask;
};

}  // namespace nn::atk::detail::driver
