#include <nn/atk/detail/AddonSoundArchiveContainer.h>

namespace nn::atk::detail {

AddonSoundArchiveContainer::AddonSoundArchiveContainer()
    : _10(false), mSoundArchive(nullptr), mSoundDataManager(nullptr) {}

// NON_MATCHING: the bounded name checks are unrolled differently.
bool AddonSoundArchiveContainer::IsSameName(const char* name) const {
    int length = 0;
    while (length < 64 && name[length] != '\0')
        ++length;
    if (length == 64)
        return false;
    for (int i = 0; i < 64; ++i) {
        if (name[i] != mName[i])
            return false;
        if (name[i] == '\0')
            return true;
    }
    return true;
}

}  // namespace nn::atk::detail
