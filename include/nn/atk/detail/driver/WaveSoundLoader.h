#pragma once

#include <nn/atk/detail/Task.h>

namespace nn::atk {
class SoundPlayer;
}

namespace nn::atk::detail {
class PlayerHeap;
}

namespace nn::atk::detail::driver {

// WaveSoundRuntime::Initialize 0x710135aec0 constructs Task subobjects at +0 and +0xa0.
class WaveSoundLoader {
public:
    bool IsInUse();
    bool TryWait();
    // LoaderManager's callback discards the result; void is inferred.
    void Finalize();

private:
    class DataLoadTask : public Task {
    public:
        ~DataLoadTask() override;
        void Execute(TaskProfileLogger& logger) override;

    private:
        friend class WaveSoundLoader;
        u8 _48[0x58 - 0x48];
        SoundPlayer* mSoundPlayer;  // +0x58, copied from the loader argument
        u8 _60[0x88 - 0x60];
        PlayerHeap* mPlayerHeap;  // +0x88
        u8 _90[0xa0 - 0x90];
    };

    class FreePlayerHeapTask : public Task {
    public:
        ~FreePlayerHeapTask() override;
        void Execute(TaskProfileLogger& logger) override;

    private:
        friend class WaveSoundLoader;
        // Execute reads the last pointer at +0x80; the heap manager follows at loader +0x128.
        u8 _48[0x78 - 0x48];
        PlayerHeap* mPlayerHeap;  // +0x78
        u8 _80[0x88 - 0x80];
    };
    static_assert(sizeof(DataLoadTask) == 0xa0);
    static_assert(sizeof(FreePlayerHeapTask) == 0x88);

    DataLoadTask mDataLoadTask;
    FreePlayerHeapTask mFreePlayerHeapTask;
};

}  // namespace nn::atk::detail::driver
