/**
 * @file TextureInfo.h
 * @brief Texture reference used by UI materials.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
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

// The object referenced by a TexMap (Material::GetTexMapArray()[i].GetTextureInfo()). Layout and vtable evidence:
// the object embedded in uking::ui::UiTexSlots (vtable 0x2477bd8, 0x18 bytes; the ctor stores the descriptor slot
// as -1 and the size as 0), eui::SetTextureInfoFromTexMap (0x7100befa74: invalidates +8, then copies +8 of the
// source), eui::SetupTextureInfoByAglTextureData (0x7100bed0f8: stores two u16 at +0x10 / +0x12),
// Picture::Append (calls virtual slot 4 = GetSize).
//
// The binary's RuntimeTypeInfo for this class has a parent (a root object at 0x25d8738 that is also the parent
// of eui::TagProcessor); that root is not known, so the class is modelled as a root here.
// Virtual slot names: GetRuntimeTypeInfo / GetSize have evidence, the rest are placeholders.
class TextureInfo {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    TextureInfo() = default;
    virtual ~TextureInfo() = default;

    virtual void m3(gfx::Device*) {}
    virtual TexSize GetSize() const { return TexSize(mWidth, mHeight); }
    virtual bool m5() const { return false; }
    virtual bool IsValid() const { return mDescriptorSlot.IsValid(); }
    virtual const void* m7() const { return nullptr; }

    const gfx::DescriptorSlot& GetDescriptorSlot() const { return mDescriptorSlot; }
    void SetDescriptorSlot(const gfx::DescriptorSlot& slot) { mDescriptorSlot = slot; }
    void InvalidateDescriptorSlot() { mDescriptorSlot.Invalidate(); }

    void SetSize(u16 width, u16 height) {
        mWidth = width;
        mHeight = height;
    }

private:
    gfx::DescriptorSlot mDescriptorSlot;
    u16 mWidth = 0;
    u16 mHeight = 0;
};
static_assert(sizeof(TextureInfo) == 0x18);

}  // namespace nn::ui2d
