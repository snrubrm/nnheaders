#include <nn/atk/SoundDataManager.h>

namespace nn {
namespace atk {
namespace detail {

void SoundArchiveLoader::SetSoundArchive(const SoundArchive* archive) {
    mArchive = archive;
}

}  // namespace detail
}  // namespace atk
}  // namespace nn
