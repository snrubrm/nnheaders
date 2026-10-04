#include <nn/ui2d/ResourceAccessor.h>

namespace nn::ui2d {

// 0x710132b160
ResourceAccessor::ResourceAccessor() = default;

// 0x710132b158 / 132b15c
ResourceAccessor::~ResourceAccessor() = default;

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
