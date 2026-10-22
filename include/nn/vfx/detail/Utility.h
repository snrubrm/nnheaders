#pragma once

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Sampler.h>
#include <nn/os/os_MutexTypes.h>
#include <nn/types.h>

namespace nn::vfx {
class Heap;
struct RenderStateSetArg;
}

namespace nn::vfx::detail {

void OutputError(const char* format, ...);
void OutputWarning(const char* format, ...);
void OutputLog(const char* format, ...);

// 0x7100ae5a24: the default render-state-set callback accepts every state set.
bool DummyRenderStateSetCallback(RenderStateSetArg& arg);

// 0x7100acac80: the destructor finalizes the wrapped OS mutex.
class Mutex {
public:
    ~Mutex();

private:
    os::MutexType m_Mutex;
};

enum class RuntimeWarningId;

// 0x7100acb04c: the release build does not report runtime warnings.
void Warning(void* handle, RuntimeWarningId id);

// 0x7100acb00c: stores the flag behind OutputLog/OutputWarning.
void SetSuppressOutputLog(bool suppress);

// The sampler table is indexed by three bytes of the resource record; each entry is 0xa8 bytes
// and holds a gfx sampler at +0x30.
struct ResTextureSampler {
    u8 _0[8];
    u8 _8;
    u8 _9;
    u8 _a;
    u8 _b[0x30 - 0xb];
    gfx::TSampler<gfx::ApiVariationNvn8> mSampler;
};
static_assert(sizeof(ResTextureSampler) == 0xa8);

class TextureSampler {
public:
    // 0x7100ad3578: index = _9 + (_8 << 2) + (_a << 4).
    static ResTextureSampler* GetSamplerFromTable(ResTextureSampler* sampler);
    // 0x7100ad34dc: finalizes the 32 table entries and frees the table.
    static void FinalizeSamplerTable(gfx::TDevice<gfx::ApiVariationNvn8>* device, Heap* heap);
};

// The two heaps behind the vfx allocator wrappers. The static heap serves the
// long-lived allocations and tracks their total size; the dynamic heap serves
// the per-frame ones. SetStaticHeap 0x7100acac84 / GetStaticHeap 0x7100acacc0 /
// SetDynamicHeap 0x7100acad58 / GetDynamicHeap 0x7100acad68 read the same two
// file-scope pointers that the wrappers below use.
void SetStaticHeap(Heap* heap);
Heap* GetStaticHeap();
void SetDynamicHeap(Heap* heap);
Heap* GetDynamicHeap();

// 0x7100acacd0: rounds the size up to 0x20 and charges it to the static total.
void* AllocFromStaticHeap(size_t size, size_t alignment);
// 0x7100acad78: rounds the size up to 0x100 and allocates from the dynamic heap.
void* AllocFromDynamicHeap(size_t size, size_t alignment);
// 0x7100acad28: frees through the static heap.
void FreeFromStaticHeap(void* ptr);
// 0x7100acadcc: frees at once, or parks the pointer on the delay-free list.
void FreeFromDynamicHeap(void* ptr, bool immediate);
// 0x7100acad48: the total charged to the static heap so far.
u64 GetAllocatedSizeFromStaticHeap();

// 0x7100acae48: allocates the delay-free list for `count` pointers.
void InitializeDelayFreeList(s32 count);
// 0x7100acaed4: frees the parked pointers and the list itself.
void FinalizeDelayFreeList();
// 0x7100acaf84: frees the parked pointers, keeping the list.
void FlushDelayFreeList();

}  // namespace nn::vfx::detail
