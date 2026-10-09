#pragma once

#include <nn/gfx/detail/gfx_Buffer-api.nvn.8.h>
#include <nn/gfx/gfx_Common.h>

namespace nn::gfx {

template <class TTarget>
class TBuffer : public detail::BufferImpl<TTarget> {
    NN_NO_COPY(TBuffer);

public:
    typedef BufferInfo InfoType;

    static const bool IsMemoryPoolRequired = true;

    // inline-only in the original; forwards to the platform implementation in
    // DispStringBuffer GPU accounting (0x71013240b4) and TextBox alignment (0x7100abcb10).
    static size_t GetBufferAlignment(TDevice<TTarget>* device, const InfoType& info) {
        return detail::BufferImpl<TTarget>::GetBufferAlignment(device, info);
    }

    TBuffer();
    void Initialize(TDevice<TTarget>*, const InfoType&, TMemoryPool<TTarget>*, ptrdiff_t, size_t);
    // inline-only in the original: RectDrawer (0x71013250d0) and GraphicsResource (0x7100ac13a8) call the platform API.
    void Finalize(TDevice<TTarget>* device) { detail::BufferImpl<TTarget>::Finalize(device); }
    void* Map() const;
    void Unmap() const;
    void FlushMappedRange(ptrdiff_t, size_t) const;
    void InvalidateMappedRange(ptrdiff_t, size_t) const;
    void GetGpuAddress(GpuAddress*) const;
    void SetUserPtr(void*);
    void* GetUserPtr();
    const void* GetUserPtr() const;
};

}  // namespace nn::gfx