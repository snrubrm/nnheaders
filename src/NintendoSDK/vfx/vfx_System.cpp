#include <nn/vfx/System.h>

#include <nn/vfx/Resource.h>
#include <nn/vfx/EmitterSet.h>
#include <nn/vfx/Heap.h>
#include <nn/vfx/detail/Utility.h>

namespace nn {
namespace vfx {

System::RenderStateSetCallback System::GetDrawPathRenderStateSetCallback(DrawPathFlag flag) {
    const u32 mask = static_cast<u32>(flag);
    if (!mask)
        return nullptr;
    for (int i = 0; i < 8; ++i) {
        if (mDrawPathFlags[i] & mask)
            return mDrawPathCallbacks[i];
    }
    return nullptr;
}

bool System::CreateEmitterSetId(Handle* handle, int emitter_set_id, int resource_id, int group,
                                Heap* heap, bool flag) {
    return CreateEmitterSetId(handle, emitter_set_id, resource_id, group, 0, heap, flag);
}

void System::InitializeEmitter(Emitter* emitter) {
    emitter->mCalculator = mEmitterCalculator;
}

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

// NON_MATCHING: link-clear stores and address scheduling differ.
void System::RemoveEmitterSetList(EmitterSet* set) {
    EmitterSet* next = set->_1c0;
    if (next || set->_1c8 || !set->_f) {
        if (_a8[set->_14] == set) {
            _a8[set->_14] = next;
            if (_a8[set->_14])
                _a8[set->_14]->_1c8 = nullptr;
            if (_2a8[set->_14] == set)
                _2a8[set->_14] = nullptr;
        } else {
            if (_2a8[set->_14] == set)
                _2a8[set->_14] = set->_1c8;
            if (set->_1c0)
                set->_1c0->_1c8 = set->_1c8;
            if (set->_1c8)
                set->_1c8->_1c0 = set->_1c0;
            else
                detail::OutputError("EmitterSet Remove Failed.\n");
        }
    } else {
        if (_4a8[set->_14] == set) {
            _4a8[set->_14] = next;
            if (_4a8[set->_14])
                _4a8[set->_14]->_1c8 = nullptr;
            if (_6a8[set->_14] == set)
                _6a8[set->_14] = nullptr;
        } else {
            if (_6a8[set->_14] == set)
                _6a8[set->_14] = set->_1c8;
            if (set->_1c0)
                set->_1c0->_1c8 = set->_1c8;
            if (set->_1c8)
                set->_1c8->_1c0 = set->_1c0;
            else
                detail::OutputError("EmitterSet Remove Failed.\n");
        }
        set->_f = 0;
    }
    set->_1c0 = nullptr;
    set->_1c8 = nullptr;
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
