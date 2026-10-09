#pragma once

#include <nn/gfx/detail/gfx_Sampler-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>

namespace nn::gfx {

template <class TTarget>
class TSampler : public detail::SamplerImpl<TTarget> {
    NN_NO_COPY(TSampler);

public:
    typedef SamplerInfo InfoType;

    TSampler();
    void Initialize(TDevice<TTarget>*, const InfoType&);
    // inline-only in the original: RectDrawer (0x71013250d0) and GraphicsResource (0x7100ac13a8) call the platform API.
    void Finalize(TDevice<TTarget>* device) { detail::SamplerImpl<TTarget>::Finalize(device); }
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

}  // namespace nn::gfx