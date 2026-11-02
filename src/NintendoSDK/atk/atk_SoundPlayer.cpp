#include <nn/atk/SoundPlayer.h>
#include <nn/atk/detail/PlayerHeap.h>

namespace nn::atk {

// No direct native caller was found; void is inferred.
void SoundPlayer::detail_AppendPlayerHeap(detail::PlayerHeap* heap) {
    heap->mPlayer = this;
    mPlayerHeapList.push_back(heap->mPlayerLink);
    ++_48;
}

// No direct native caller was found; void is inferred.
void SoundPlayer::detail_FreePlayerHeap(detail::PlayerHeap* heap) {
    if (heap->_28 == 2)
        mPlayerHeapList.push_back(heap->mPlayerLink);
    else
        mOtherPlayerHeapList.push_back(heap->mPlayerLink);
}

}  // namespace nn::atk
