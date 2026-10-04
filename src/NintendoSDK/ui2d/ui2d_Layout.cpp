#include <nn/ui2d/Layout.h>

namespace nn::ui2d {

Layout::AllocateFunction Layout::g_pAllocateFunction;
Layout::FreeFunction Layout::g_pFreeFunction;
void* Layout::g_pUserData;

// 0x7100ab65cc
void Layout::SetAllocator(AllocateFunction allocate, FreeFunction free, void* user_data) {
    g_pAllocateFunction = allocate;
    g_pFreeFunction = free;
    g_pUserData = user_data;
}

// 0x7100ab65f4
void* Layout::AllocateMemory(size_t size, size_t alignment) {
    return g_pAllocateFunction(size, alignment, g_pUserData);
}

// 0x7100ab6610
void Layout::FreeMemory(void* ptr) {
    g_pFreeFunction(ptr, g_pUserData);
}

}  // namespace nn::ui2d
