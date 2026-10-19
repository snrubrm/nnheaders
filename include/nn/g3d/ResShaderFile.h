#pragma once

#include <nn/types.h>

namespace nn::g3d {

// The resource layout remains opaque. Only native static entrypoints are declared.
class ResShaderFile {
public:
    static bool IsValid(const void* data);
    static ResShaderFile* ResCast(void* data);
};

// The class name comes from the original symbol. Only the four bytes at
// +0x1d..+0x23 are recovered: the dynamic base index, the key index, the
// shift and the mask of the shader option. The extent of the original class
// is not known.
class ResShaderOption {
public:
    u32 ReadStaticKey(const u32* keys) const;
    void WriteStaticKey(u32* keys, int value) const;
    void WriteDynamicKey(u32* keys, int value) const;

private:
    u8 _0[0x1d];
    u8 _1d;   // the base index of a dynamic key
    u8 _1e;   // the key index
    u8 _1f;   // the shift
    u32 _20;  // the mask
};
static_assert(sizeof(ResShaderOption) == 0x24);

}  // namespace nn::g3d
