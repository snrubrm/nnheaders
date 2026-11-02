#include <nn/atk/detail/driver/SequenceSoundLoader.h>

namespace nn::atk::detail::driver {

bool SequenceSoundLoader::IsInUse() {
    return !mDataLoadTask.IsFinishedMaybe() || !mFreePlayerHeapTask.IsFinishedMaybe();
}

}  // namespace nn::atk::detail::driver
