#pragma once

#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/ui2d/ShaderInfo.h>

namespace nn::ui2d {

class ShaderInfo;

class ShaderContainer {
public:
    ShaderContainer() = default;
    ~ShaderContainer();

    void Finalize(gfx::Device* device);
    ShaderInfo* FindShaderByName(const char* name) const;
    ShaderInfo* RegisterShader(const char* name, bool flag);

private:
    // RegisterShader allocates 0x60 bytes and returns the ShaderInfo at +0x18.
    struct Entry : nn::util::IntrusiveListNode {
        explicit Entry(bool own_resource) : mOwnResource(own_resource) { mName[0] = '\0'; }

        char mName[8];
        ShaderInfo mShaderInfo;
        bool mOwnResource;
    };
    static_assert(sizeof(Entry) == 0x60);

    nn::util::IntrusiveList<Entry, nn::util::IntrusiveListBaseNodeTraits<Entry>> mEntries;
};

}  // namespace nn::ui2d
