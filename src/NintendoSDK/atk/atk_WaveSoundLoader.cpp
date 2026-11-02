#include <nn/atk/detail/driver/WaveSoundLoader.h>

namespace nn::atk::detail::driver {

bool WaveSoundLoader::IsInUse() {
    return !mDataLoadTask.IsFinishedMaybe() || !mFreePlayerHeapTask.IsFinishedMaybe();
}

}  // namespace nn::atk::detail::driver
