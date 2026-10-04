#include <nn/ui2d/Layout.h>
#include <nn/util.h>

namespace nn::ui2d {

void sub_710132B114(Layout::AllocateFunction allocate, Layout::FreeFunction free, void* user_data) {
    nn::util::ReferSymbol("SDK MW+Nintendo+NintendoWare_Ui2d-4_4_0-Release");
    Layout::SetAllocator(allocate, free, user_data);
}

}  // namespace nn::ui2d
