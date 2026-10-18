#pragma once

#include <nn/types.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Types.h>

namespace nn::g3d {

class ResMaterial;
class ResModel;
class ShapeObj;
class SkeletonObj;

// Graphics 0x7100f2e0f4 walks ModelObj's material array with a 0x80-byte stride and reads the
// resource material through +0; agl::g3d::MaterialObj::GetResTexture 0x7100b33fbc reads +0x50.
class MaterialObj {
public:
    const ResMaterial* GetResource() const { return m_pRes; }

private:
    const ResMaterial* m_pRes;
    u8 _8[0x50 - 0x8];

public:
    // AGL MaterialObj bindings 0x7100b33fbc/0x7100b33fd8 read the view array at
    // +0x50 and 0x7100b34024 writes the view and descriptor slot arrays; the
    // change callback at +0x78 is called with the material and index.
    const gfx::TextureView** mTextureViews;  // 50
    gfx::DescriptorSlot* mTextureDescriptorSlots;  // 58

private:
    u8 _60[0x78 - 0x60];

public:
    void (*mTextureChangeCallback)(MaterialObj*, s32);  // 78
};
static_assert(sizeof(MaterialObj) == 0x80);

// TODO
class ModelObj {
public:
    SkeletonObj* GetSkeleton() const { return m_Skeleton; }

    // Inline-only in the original; name is a guess. ModelNW::getName
    // 0x7100c06bc0 and ModelRig 0x7100c8338c read this actual resource.
    const ResModel* GetResource() const { return m_ResModel; }

    s32 GetNumShapes() const { return m_NumShapes; }
    // Graphics 0x7100f2e0f4: the material count (+0x32) and array (+0x48).
    s32 GetNumMaterials() const { return m_NumMaterials; }
    MaterialObj* GetMaterial(int index) { return &m_Materials[index]; }

    // inline-only in the original; name is a guess: +58 user pointer in ModelNW Bone/MaterialVisibilityCallback c04134/c04164.
    void* GetUserPtr() { return m_UserData; }

    s32 get_8c() const { return _8c; }

private:
    struct InitializeArgument;

    bool Initialize(const InitializeArgument& arg, void* buffer, size_t bufferSize);

    const ResModel* m_ResModel;
    void* _8;
    void* _10;
    u8 _18;
    u8 _19;
    u16 _1a;
    void* _20;
    void* _28;
    u16 m_NumShapes;
    u16 m_NumMaterials;
    SkeletonObj* m_Skeleton;
    ShapeObj* m_Shapes;
    MaterialObj* m_Materials;
    void* _50;
    void* m_UserData;
    void* _60;
    void* _68;
    void* _70;
    void* _78;
    void* _80;
    bool _88;
    int _8c;
};

}  // namespace nn::g3d
