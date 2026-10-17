/**
 * @file System.h
 * @brief VFX system implementation.
 */

#pragma once

#include <nn/types.h>
#include <nn/vfx/Config.h>

// Partial member model. The derived PtclSystem starts its own fields at 0x17b0.
namespace nn {
namespace vfx {

class Heap;

class System {
public:
    System(nn::vfx::Config const&);

    virtual ~System();
    virtual void Initialize(nn::vfx::Heap*, nn::vfx::Heap*, nn::vfx::Config const&);

    // 0x7100accbac: kills the emitters of all 64 emitter sets and clears the two list heads.
    void KillAllEmitterSet();

private:
    // SDK constructor 0x7100acb47c constructs the last mutex at +0x1790 (size 0x20).
    // PtclSystem constructor 0x7100c51c3c starts its own tick at +0x17b0.
    u8 _8[0x17b0 - 8];
};
static_assert(sizeof(System) == 0x17b0);
}  // namespace vfx
}  // namespace nn
