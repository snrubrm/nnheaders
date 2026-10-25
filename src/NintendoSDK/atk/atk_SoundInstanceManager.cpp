#include <nn/atk/detail/AdvancedWaveSoundRuntime.h>
#include <nn/atk/detail/SequenceSoundRuntime.h>
#include <nn/atk/detail/SoundInstanceManager.h>
#include <nn/atk/detail/WaveSoundRuntime.h>

namespace nn {
namespace atk {
namespace detail {

void AdvancedWaveSoundRuntime::Update() {
    SortPriorityList();
}

void SequenceSoundRuntime::Update() {
    SortPriorityList();
}

void WaveSoundRuntime::Update() {
    SortPriorityList();
}

}  // namespace detail
}  // namespace atk
}  // namespace nn
