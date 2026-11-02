#include <nn/atk/SoundPlayer.h>
#include <nn/atk/detail/PlayerHeap.h>

namespace nn::atk {

// No direct native caller was found; void is inferred.
void SoundPlayer::detail_AppendPlayerHeap(detail::PlayerHeap* heap) {
    heap->mPlayer = this;
    mPlayerHeapList.push_back(heap->mPlayerLink);
    ++_48;
}

}  // namespace nn::atk
