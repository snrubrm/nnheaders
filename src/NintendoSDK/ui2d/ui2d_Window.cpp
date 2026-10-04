#include <nn/ui2d/Window.h>

namespace nn::ui2d {

// The original preserves Window's vtable store before calling Pane's own
// destructor. Keep it as in GameDataFlagSelector (commit 96101229).
Window::~Window() {
    ;
}

}  // namespace nn::ui2d
