#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::ui2d {

class ShaderInfo;

// The container has a single intrusive-list root; entry layout is not recovered here.
class ShaderContainer {
public:
    ShaderContainer();
    ~ShaderContainer();

    ShaderInfo* FindShaderByName(const char* name) const;
    ShaderInfo* RegisterShader(const char* name, bool flag);

private:
    nn::util::IntrusiveListNode mRoot;
};

}  // namespace nn::ui2d
