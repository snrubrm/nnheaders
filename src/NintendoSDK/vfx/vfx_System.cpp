#include <nn/vfx/System.h>

#include <nn/vfx/Resource.h>

namespace nn {
namespace vfx {

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
