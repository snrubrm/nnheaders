#pragma once

#include <cstddef>
#include <nn/font/font_RectDrawer.h>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_State.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::ui2d {

// The graphics objects shared by all layouts: common blend states and a pair of constant buffers
// with their memory pools. The layout is recovered from the constructor (0x7100ac1290).
// RectDrawer at +0x78 is identified by its native constructor and RTTI at 0x710252c428.
class GraphicsResource {
public:
    GraphicsResource();
    // Original 0x7100ac1334 destroys all ten modeled gfx members and the object at +0x78.
    ~GraphicsResource();

    using SamplerDescriptorCallback = void (*)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);
    // Name is a reconstruction guess paired with RegisterCommonSamplerSlot (0x7100ac16c0). 0x7100ac18bc calls this callback
    // with 0x78-byte samplers and 8-byte slots; ScreenMgr passes UnregisterSlotForSampler.
    void UnregisterCommonSamplerSlot(SamplerDescriptorCallback callback, void* user_data);

    // 0x7100ac13a8
    void Finalize(gfx::Device* device);
    // Native neighbours select a blend-state member and bind the common vertex buffer.
    const gfx::BlendState* sub_7100AC1158(u32 index) const;
    void sub_7100AC1244(gfx::CommandBuffer* command_buffer) const;

public:
    void* _0 = nullptr;
    s32 _8 = 2;
    u8 _c[4];
    // Constructor clears exactly +0x10 through +0x6b, leaving +0x0c and alignment padding untouched.
    u8 _10[0x5c]{};
    u64 _70 = 4;
    font::RectDrawer mRectDrawer;
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
