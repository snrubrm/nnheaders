#include <nn/ui2d/ShaderContainer.h>
#include <nn/ui2d/Layout.h>
#include <nn/util/util_StringUtil.h>
#include <new>

namespace nn::ui2d {

// 0x710132b284
ShaderContainer::~ShaderContainer() {}

// NON_MATCHING: existing single-node list insertion retains two extra pointer operations.
ShaderInfo* ShaderContainer::RegisterShader(const char* name, bool own_resource) {
    void* memory = Layout::AllocateMemory(sizeof(Entry), 4);
    if (!memory)
        return nullptr;
    auto* entry = new (memory) Entry(own_resource);
    util::Strlcpy(entry->mName, name, sizeof(entry->mName));
    mEntries.push_back(*entry);
    return &entry->mShaderInfo;
}

// NON_MATCHING: the existing string comparison tests the terminator before equality.
ShaderInfo* ShaderContainer::FindShaderByName(const char* name) const {
    for (const auto& entry : mEntries) {
        if (util::Strncmp(name, entry.mName, sizeof(entry.mName)) == 0)
            return const_cast<ShaderInfo*>(&entry.mShaderInfo);
    }
    return nullptr;
}

void ShaderContainer::Finalize(gfx::Device* device) {
    while (!mEntries.empty()) {
        auto& entry = mEntries.front();
        entry.mShaderInfo.Finalize(device, !entry.mOwnResource);
        mEntries.erase(mEntries.iterator_to(entry));
        entry.~Entry();
        Layout::FreeMemory(&entry);
    }
}

}  // namespace nn::ui2d
