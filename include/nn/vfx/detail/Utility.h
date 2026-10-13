#pragma once

namespace nn::vfx::detail {

void OutputError(const char* format, ...);
void OutputWarning(const char* format, ...);
void OutputLog(const char* format, ...);

}  // namespace nn::vfx::detail
