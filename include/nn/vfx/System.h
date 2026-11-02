/**
 * @file System.h
 * @brief VFX system implementation.
 */

#pragma once

#include <nn/types.h>
#include <nn/os/os_Mutex.h>
#include <nn/vfx/Config.h>

// Partial member model. The derived PtclSystem starts its own fields at 0x17b0.
namespace nn {
namespace vfx {

class Emitter;
class EmitterSet;
class Heap;
class Resource;
struct RenderStateSetArg;
// The enumerators and signedness are not established; the value is used as a 32-bit mask.
enum class DrawPathFlag : u32;

class System {
public:
    System(nn::vfx::Config const&);

    virtual ~System();
    virtual void Initialize(nn::vfx::Heap*, nn::vfx::Heap*, nn::vfx::Config const&);

    using RenderStateSetCallback = void (*)(RenderStateSetArg&);
    RenderStateSetCallback GetDrawPathRenderStateSetCallback(DrawPathFlag flag);

    void ClearResource(Heap* heap, int resource_id);
    void KillEmitterSet(EmitterSet* emitter_set, bool immediate);
    void KillEmitterSetGroup(int group);
    void RemoveEmitterSetList(EmitterSet* set);

    // 0x7100accbac: kills the emitters of all 64 emitter sets and clears the two list heads.
    void KillAllEmitterSet();

    // 0x7100acd77c: the id of the named emitter set in resource slot `resource_id`, or -1.
    int SearchEmitterSetId(const char* name, int resource_id) const;
    // 0x7100acd778: the native body is empty.
    void FlushTempBuffer();
    // Inline-only in the original; name is a guess. xlink2::OneEmitterMgr::createOneEmitterInstance
    // 0x7100bcdf6c loops over this many resource slots (+0x64).
    int GetResourceNumMaybe() const { return _64; }
    // Inline-only in the original; name is a guess. The resource slot array at +0x90
    // (xlink2::PtclResourceAccessorELink::searchEmitterSetID 0x7100bbc130).
    Resource* GetResourceMaybe(int index) const { return _90[index]; }

private:
    // SDK constructor 0x7100acb47c constructs the last mutex at +0x1790 (size 0x20).
    // PtclSystem constructor 0x7100c51c3c starts its own tick at +0x17b0.
    u8 _8[0x64 - 8];
    int _64;
    u8 _68[0x90 - 0x68];
    Resource** _90;
    u8 _98[0xa8 - 0x98];
    EmitterSet* _a8[64];
    EmitterSet* _2a8[64];
    EmitterSet* _4a8[64];
    EmitterSet* _6a8[64];
    EmitterSet** mPendingKillSets;       // +0x8a8
    s32 mPendingKillSetCount;            // +0x8b0
    u8 _8b4[0x1170 - 0x8b4];
    u32 mDrawPathFlags[8];                // +0x1170
    u8 _1190[0x10];
    RenderStateSetCallback mDrawPathCallbacks[8];  // +0x11a0
    u8 _11e0[0x1298 - 0x11e0];
    Emitter* _1298;  // Compute emitter list head.
    Emitter* _12a0;  // Compute emitter list tail.
    u8 _12a8[0x1770 - 0x12a8];
    nn::os::Mutex mPendingKillMutex;     // +0x1770
    u8 _1790[0x17b0 - 0x1790];
};
static_assert(sizeof(System) == 0x17b0);
}  // namespace vfx
}  // namespace nn
