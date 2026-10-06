#pragma once

#include <cstddef>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_State.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::ui2d {

// The graphics objects shared by all layouts: common blend states and a pair of constant buffers
// with their memory pools. The layout is recovered from the constructor (0x7100ac1290).
// TODO: incomplete. The object at 0x78 (constructed by 0x7101324f5c: vertex states, buffers, memory
// pools and samplers) is not modelled; the members are not written by any decompiled function yet.
class GraphicsResource {
public:
    GraphicsResource();

    // 0x7100ac13a8
    void Finalize(gfx::Device* device);

public:
    void* _0 = nullptr;
    s32 _8 = 2;
    u8 _c[0x70 - 0xc];
    u64 _70 = 4;
    u8 _78[0x6e0 - 0x78];
    gfx::Buffer mBuffer0;
    gfx::MemoryPool mMemoryPool0;
    gfx::Buffer mBuffer1;
    gfx::MemoryPool mMemoryPool1;
    void* _9b0 = nullptr;
    void* _9b8 = nullptr;
    gfx::BlendState mBlendStates[6];
    bool _ab0 = false;
};
static_assert(offsetof(GraphicsResource, mBuffer0) == 0x6e0);
static_assert(offsetof(GraphicsResource, mMemoryPool0) == 0x728);
static_assert(offsetof(GraphicsResource, mBuffer1) == 0x848);
static_assert(offsetof(GraphicsResource, mMemoryPool1) == 0x890);
static_assert(offsetof(GraphicsResource, mBlendStates) == 0x9c0);
static_assert(offsetof(GraphicsResource, _ab0) == 0xab0);
static_assert(sizeof(GraphicsResource) == 0xab8);

}  // namespace nn::ui2d
