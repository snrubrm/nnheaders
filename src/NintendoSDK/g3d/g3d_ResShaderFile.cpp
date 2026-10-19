#include <nn/g3d/ResShaderFile.h>
#include <nn/util/util_BinaryFormat.h>

namespace nn::g3d {

bool ResShaderFile::IsValid(const void* data) {
    // Native FSHA file signature and version 3.0.1.
    return static_cast<const util::BinaryFileHeader*>(data)->IsValid(0x2020202041485346, 3, 0, 1);
}

u32 ResShaderOption::ReadStaticKey(const u32* keys) const {
    return (keys[_1e] & _20) >> _1f;
}

void ResShaderOption::WriteStaticKey(u32* keys, int value) const {
    keys[_1e] = (keys[_1e] & ~_20) | (static_cast<u32>(value) << _1f);
}

void ResShaderOption::WriteDynamicKey(u32* keys, int value) const {
    const u64 index = _1e - _1d;
    keys[index] = (keys[index] & ~_20) | (static_cast<u32>(value) << _1f);
}

}  // namespace nn::g3d
