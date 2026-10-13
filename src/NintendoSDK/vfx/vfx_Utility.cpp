#include <nn/vfx/detail/Utility.h>

#include <cstdarg>

namespace nn::vfx::detail {

// The release implementation prepares variable arguments without emitting output.
void OutputError(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    va_end(args);
}

void OutputWarning(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    va_end(args);
}

void OutputLog(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    va_end(args);
}

}  // namespace nn::vfx::detail
