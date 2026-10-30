#include <nn/ui2d/ResourceAccessor.h>
#include <nn/font/font_ResFont.h>
#include <nn/ui2d/Layout.h>
#include <new>

namespace nn::ui2d {

static_assert(sizeof(font::ResFont) == 0xa8);

// 0x710132b160
ResourceAccessor::ResourceAccessor() = default;

// 0x710132b158 / 132b15c
ResourceAccessor::~ResourceAccessor() = default;

font::Font* ResourceAccessor::LoadFont(gfx::Device* device, const char* name) {
    size_t size = 0;
    void* resource = GetResource(&size, 0x666f6e74, name);
    if (!resource || !size)
        return nullptr;
    void* memory = Layout::AllocateMemory(sizeof(font::ResFont), 4);
    if (!memory)
        return nullptr;
    auto* font = new (memory) font::ResFont();
    if (!font->SetResource(device, resource, nullptr, 0, 0)) {
        font->~ResFont();
        Layout::FreeMemory(font);
        return nullptr;
    }
    return font;
}

// 0x710132b230
void ResourceAccessor::Finalize(gfx::Device*) {}

// 0x7100be0e00: the const overload forwards to the mutable resource operation.
void* ResourceAccessor::GetResource(size_t* size, u32 type, const char* name) const {
    return const_cast<ResourceAccessor*>(this)->GetResource(size, type, name);
}

// 0x7100be0e0c
void* ResourceAccessor::FindResourceByName(u32 type, const char* name) {
    return GetResource(nullptr, type, name);
}

// 0x7100be0e2c
void* ResourceAccessor::FindResourceByName(u32 type, const char* name) const {
    return GetResource(nullptr, type, name);
}

}  // namespace nn::ui2d
