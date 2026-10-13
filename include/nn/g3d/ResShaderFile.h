#pragma once

namespace nn::g3d {

// The resource layout remains opaque. Only native static entrypoints are declared.
class ResShaderFile {
public:
    static bool IsValid(const void* data);
    static ResShaderFile* ResCast(void* data);
};

}  // namespace nn::g3d
