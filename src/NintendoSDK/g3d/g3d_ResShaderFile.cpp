#include <nn/g3d/ResShaderFile.h>
#include <nn/util/util_BinaryFormat.h>

namespace nn::g3d {

bool ResShaderFile::IsValid(const void* data) {
    // Native FSHA file signature and version 3.0.1.
    return static_cast<const util::BinaryFileHeader*>(data)->IsValid(0x2020202041485346, 3, 0, 1);
}

}  // namespace nn::g3d
