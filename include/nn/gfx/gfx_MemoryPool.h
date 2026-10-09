#pragma once

#include <nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>

namespace nn::gfx {

template <class TTarget>
class TMemoryPool : public detail::MemoryPoolImpl<TTarget> {
    NN_NO_COPY(TMemoryPool);

public:
    typedef MemoryPoolInfo InfoType;

    static size_t GetPoolMemoryAlignment(TDevice<TTarget>*, const InfoType&);
    static size_t GetPoolMemorySizeGranularity(TDevice<TTarget>*, const InfoType&);

    TMemoryPool();

    void Initialize(TDevice<TTarget>*, const InfoType&);
    // inline-only in the original: RectDrawer (0x71013250d0) and GraphicsResource (0x7100ac13a8) call the platform API.
    void Finalize(TDevice<TTarget>* device) { detail::MemoryPoolImpl<TTarget>::Finalize(device); }
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

}  // namespace nn::gfx
