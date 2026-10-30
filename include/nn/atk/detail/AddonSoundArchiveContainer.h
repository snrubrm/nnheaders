#pragma once

#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {
class SoundArchive;
class SoundDataManager;

namespace detail {

class AddonSoundArchiveContainer : public nn::util::IntrusiveListNode {
public:
    AddonSoundArchiveContainer();
    bool IsSameName(const char* name) const;

    bool _10;
    const SoundArchive* mSoundArchive;
    const SoundDataManager* mSoundDataManager;
    char mName[64];
};
static_assert(sizeof(AddonSoundArchiveContainer) == 0x68);

}  // namespace detail
}  // namespace nn::atk
