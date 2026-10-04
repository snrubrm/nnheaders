#include <nn/ui2d/Layout.h>

#include <nn/ui2d/TextBox.h>

namespace nn::ui2d {

// 0x7100ab79a8 (placeholder name; the CSV row is unnamed): sets the tag processor of every text box in the pane tree
void sub_7100AB79A8(Pane* pane, font::TagProcessorBase<u16>* tag_processor) {
    if (TextBox* text_box = font::DynamicCast<TextBox>(pane))
        text_box->SetTagProcessor(tag_processor);
    for (Pane& child : pane->GetChildList())
        sub_7100AB79A8(&child, tag_processor);
}

Layout::AllocateFunction Layout::g_pAllocateFunction;
Layout::FreeFunction Layout::g_pFreeFunction;
void* Layout::g_pUserData;

// 0x7100ab65cc
void Layout::SetAllocator(AllocateFunction allocate, FreeFunction free, void* user_data) {
    g_pAllocateFunction = allocate;
    g_pFreeFunction = free;
    g_pUserData = user_data;
}

// 0x7100ab79a0
void Layout::SetTagProcessor(font::TagProcessorBase<u16>* tag_processor) {
    sub_7100AB79A8(mPane, tag_processor);
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
