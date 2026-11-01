#include <nn/vfx/System.h>

#include <nn/vfx/Resource.h>
#include <nn/vfx/Heap.h>
#include <nn/vfx/detail/Utility.h>

namespace nn {
namespace vfx {

void System::ClearResource(Heap* heap, int resource_id) {
    if (_90[resource_id]) {
        _90[resource_id]->Finalize(heap);
        heap->Free(_90[resource_id]);
        _90[resource_id] = nullptr;
    } else {
        detail::OutputWarning("no resource to delete.");
    }
}

void System::FlushTempBuffer() {
}

int System::SearchEmitterSetId(const char* name, int resource_id) const {
    Resource* resource = _90[resource_id];
    if (!resource || !resource->IsAvailableMaybe())
        return -1;
    return resource->SearchEmitterSetId(name);
}

}  // namespace vfx
}  // namespace nn
