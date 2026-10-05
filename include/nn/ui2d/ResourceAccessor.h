#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::font {
class Font;
}

namespace nn::ui2d {

class TextureInfo;
class ResourceTextureInfo;
class ShaderInfo;

// The prefix through shader loading is recovered from the base table (0x710252c8e8)
// and eui::MultiArcResourceAccessor (0x71024c7ee0). Later virtuals remain unmodelled.
class ResourceAccessor {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    ResourceAccessor();
    virtual ~ResourceAccessor();

    using TextureViewDescriptorCallback = void (*)(gfx::DescriptorSlot*, const gfx::TextureView&,
                                                   void*);

    // Names of these two descriptor operations follow the original named unregister override.
    virtual void RegisterTextureViewToDescriptorPool(TextureViewDescriptorCallback, void*) = 0;
    virtual void UnregisterTextureViewFromDescriptorPool(TextureViewDescriptorCallback, void*) = 0;
    virtual void Finalize(gfx::Device* device);

    // Descriptive names: the resource operation writes an optional byte size and returns
    // mutable archive storage, also passed to ResFont::SetResource(void*).
    virtual void* GetResource(size_t* size, u32 type, const char* name) = 0;
    virtual void* GetResource(size_t* size, u32 type, const char* name) const;
    virtual void* FindResourceByName(u32 type, const char* name);
    virtual void* FindResourceByName(u32 type, const char* name) const;
    virtual font::Font* AcquireFont(gfx::Device* device, const char* name) = 0;
    // Base slot 0x58 is pure; MultiArcResourceAccessor returns its embedded TextureInfo.
    virtual TextureInfo* AcquireTexture(gfx::Device* device, const char* name) = 0;
    virtual ShaderInfo* AcquireShader(gfx::Device* device, const char* name) = 0;
    virtual bool LoadTexture(ResourceTextureInfo* texture, gfx::Device* device,
                             const char* name) = 0;
    // The original slot 14 loads a font; its source name is unknown.
    virtual font::Font* sub_710132B174(gfx::Device* device, const char* name);
    virtual bool LoadShader(ShaderInfo* shader, gfx::Device* device, const char* name) = 0;
};
static_assert(sizeof(ResourceAccessor) == 8);

}  // namespace nn::ui2d
