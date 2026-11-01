#include <nn/vfx/System.h>

#include <nn/vfx/Resource.h>
#include <nn/vfx/EmitterSet.h>
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

void System::KillEmitterSet(EmitterSet* emitter_set, bool) {
    if (!emitter_set->IsActiveMaybe()) {
        detail::OutputWarning("EmitterSet is already removed.\n");
        return;
    }

    mPendingKillMutex.Lock();
    const s32 count = mPendingKillSetCount;
    EmitterSet** sets = mPendingKillSets;
    s64 i = 0;
    for (; i < count; ++i) {
        if (sets[i] == emitter_set)
            break;
    }
    if (i >= count) {
        sets[count] = emitter_set;
        ++mPendingKillSetCount;
    }
    mPendingKillMutex.Unlock();
}

void System::KillEmitterSetGroup(int group) {
    EmitterSet* set = _a8[group];
    while (set) {
        EmitterSet* next = set->GetNextMaybe();
        KillEmitterSet(set, false);
        set = next;
    }

    set = _4a8[group];
    while (set) {
        EmitterSet* next = set->GetNextMaybe();
        KillEmitterSet(set, false);
        set->ClearListFlagMaybe();
        set = next;
    }
}

void System::KillAllEmitterSet() {
    for (int group = 0; group < 64; ++group)
        KillEmitterSetGroup(group);
    _12a0 = nullptr;
    _1298 = nullptr;
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
