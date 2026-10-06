/**
 * @file TextureInfo.h
 * @brief Texture reference used by UI materials.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Texture.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/types.h>

namespace nn::ui2d {

// Width and height of a texture. Returned by value through a hidden pointer (Picture::Append 0x7100ab9e8c
// calls the virtual and reads two halfwords from the result buffer), hence the user-provided copy constructor.
struct TexSize {
    u16 width;
    u16 height;

    TexSize() = default;
    TexSize(u16 w, u16 h) : width(w), height(h) {}
    TexSize(const TexSize& other) : width(other.width), height(other.height) {}
};

// The root of the texture reference classes (the object a TexMap points to,
// Material::GetTexMapArray()[i].GetTextureInfo()). Evidence: the type is the parameter of Picture::Append,
// TexMap::TexMap, eui::SetTextureInfoFromTexMap and the return type of ResourceAccessor::AcquireTexture, while
// the two concrete classes below are siblings in the binary's RuntimeTypeInfo chain (statics 0x25d8748 and
// 0x25fca88, both with the parent static 0x25d8738 that this class models). The descriptor slot at +8 is shared by
// both (eui::SetTextureInfoFromTexMap 0x7100befa74 invalidates it and copies the source's through this type).
// Virtual slot names: GetRuntimeTypeInfo / Finalize / GetSize / IsValid / GetTextureView have evidence (the
// ResourceTextureInfo CSV name, Picture::Append calling slot 4, the resource implementation), the rest are
// placeholders.
class TextureInfo {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    TextureInfo() = default;
    virtual ~TextureInfo() = default;

    virtual void Finalize(gfx::Device* device) = 0;
    virtual TexSize GetSize() const = 0;
    virtual u32 m5() const = 0;
    virtual bool IsValid() const = 0;
    virtual const gfx::TextureView* GetTextureView() const = 0;
    virtual gfx::TextureView* GetTextureView() = 0;

    gfx::DescriptorSlot& GetDescriptorSlot() { return mDescriptorSlot; }
    const gfx::DescriptorSlot& GetDescriptorSlot() const { return mDescriptorSlot; }
    void SetDescriptorSlot(const gfx::DescriptorSlot& slot) { mDescriptorSlot = slot; }
    void InvalidateDescriptorSlot() { mDescriptorSlot.Invalidate(); }

protected:
    gfx::DescriptorSlot mDescriptorSlot;
};
static_assert(sizeof(TextureInfo) == 0x10);

// A texture reference whose texture is owned elsewhere: it only holds a descriptor slot and a size. The object
// embedded in uking::ui::UiTexSlots (vtable 0x2477bd8; the ctor stores the descriptor slot as -1 and the size as
// 0), eui::SetupTextureInfoByAglTextureData (0x7100bed0f8) stores two u16 at +0x10 / +0x12 after a DynamicCast
// to this class. The class name is a guess (the binary has no name for it).
class ExternalTextureInfo : public TextureInfo {
public:
    NN_RUNTIME_TYPEINFO(TextureInfo)

    ExternalTextureInfo() = default;
    ~ExternalTextureInfo() override = default;

    void Finalize(gfx::Device*) override {}
    TexSize GetSize() const override { return mSize; }
    u32 m5() const override { return 0; }
    bool IsValid() const override { return mDescriptorSlot.IsValid(); }
    const gfx::TextureView* GetTextureView() const override { return nullptr; }
    gfx::TextureView* GetTextureView() override { return nullptr; }

    void SetSize(u16 width, u16 height) {
        mSize.width = width;
        mSize.height = height;
    }

private:
    TexSize mSize{0, 0};
};
static_assert(sizeof(ExternalTextureInfo) == 0x18);

}  // namespace nn::ui2d
