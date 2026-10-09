#pragma once

#include <cstddef>
#include <nn/gfx/gfx_Buffer.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_MemoryPool.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/gfx/gfx_State.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::gfx {
class ResShaderFile;
}

namespace nn::font {

// RTTI at 0x710252c428 names RectDrawer. Constructor 0x7101324f5c and destructor
// 0x7101325004 establish every GPU member below. GraphicsResource embeds the 0x668-byte object.
class RectDrawer {
public:
    RectDrawer();
    virtual ~RectDrawer();
    virtual void Finalize(gfx::Device* device);

    using SamplerRegistrationCallback = bool (*)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);
    using SamplerDescriptorCallback = void (*)(gfx::DescriptorSlot*, const gfx::Sampler&, void*);
    // Native neighbours 0x71013252fc / 0x7101325310 pass the sampler and descriptor to the callback.
    void RegisterSamplerSlot(SamplerRegistrationCallback callback, void* user_data);
    void UnregisterSamplerSlot(SamplerDescriptorCallback callback, void* user_data);

private:
    // Initialize copies the BNSH shader file at 0x7102524000, then stores shader selection
    // and five six-entry slot arrays. ResCast at 0x7101320d70 has a shared relocation-only body.
    gfx::ResShaderFile* mResShaderFile;
    s32 mShaderSelection;
    s32 mVertexUniformSlots[6];
    s32 mVertexUnorderedAccessSlots[6];
    s32 mPixelUniformSlots[6];
    s32 mPixelInterpolationSlots[6];
    s32 mTextureSlots[6];
    // Draw checks this capacity against DispStringBuffer::mCharCount; Initialize sets the supplied capacity.
    s32 mMaxCharCount;
    gfx::VertexState mVertexStates[6];
    gfx::Buffer mVertexBuffer;
    gfx::Buffer mIndexBuffer;
    gfx::MemoryPool mVertexMemoryPool;
    // Initialize stores 1 and 0 in these buffers; drawing selects one for black-white interpolation.
    gfx::Buffer mInterpolationEnabledBuffer;
    gfx::MemoryPool mInterpolationEnabledMemoryPool;
    u64 mInterpolationEnabledBufferSize;
    gfx::Buffer mInterpolationDisabledBuffer;
    gfx::MemoryPool mInterpolationDisabledMemoryPool;
    u64 mInterpolationDisabledBufferSize;
    gfx::Sampler mSampler;
    gfx::DescriptorSlot mSamplerSlot;
    // Initialize stores its incoming work-memory pointer at +0x660; the constructor leaves it untouched.
    void* mWorkMemory;
};
static_assert(sizeof(RectDrawer) == 0x668);

}  // namespace nn::font
