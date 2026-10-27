#pragma once

#include <nn/types.h>
#include <nn/util/MathTypes.h>
#include <math/seadVector.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/g3d/ResShape.h>

namespace nn::g3d {

class ResMaterial;
class ResModel;
class ShapeObj;
class SkeletonObj;

// The bound sphere cached on a ShapeObj at +0x38. gsys::ModelRenderUnitNW::getBoundSphereImpl
// 0x7100c0a530 reads the center from +0, +4 and +8 and the radius from +0x10.
class Sphere {
public:
    sead::Vector3f mCenter;
    f32 _c;
    f32 mRadius;
};
static_assert(offsetof(Sphere, mRadius) == 0x10);

// Graphics 0x7100f2e0f4 walks ModelObj's material array with a 0x80-byte stride and reads the
// resource material through +0; agl::g3d::MaterialObj::GetResTexture 0x7100b33fbc reads +0x50.
class MaterialObj {
public:
    const ResMaterial* GetResource() const { return m_pRes; }

private:
    const ResMaterial* m_pRes;

public:
    // AGL bindings 0x7100b340ec/0x7100b34114 read the flag byte at +8, the
    // 0x48-byte GPU buffer array at +0x40 and the mapped size at +0x60.
    u8 mFlags;  // 8

private:
    u8 _9[0x40 - 0x9];

public:
    gfx::Buffer* mBuffers;  // 40

private:
    u8 _48[0x50 - 0x48];

public:
    // AGL MaterialObj bindings 0x7100b33fbc/0x7100b33fd8 read the view array at
    // +0x50 and 0x7100b34024 writes the view and descriptor slot arrays; the
    // change callback at +0x78 is called with the material and index.
    const gfx::TextureView** mTextureViews;  // 50
    gfx::DescriptorSlot* mTextureDescriptorSlots;  // 58
    size_t mBufferSize;  // 60

private:
    u8 _68[0x78 - 0x68];

public:
    void (*mTextureChangeCallback)(MaterialObj*, s32);  // 78
};
static_assert(sizeof(MaterialObj) == 0x80);

// Prefix used by the AGL ShapeObj::Map binding 0x7100b34184. The total extent
// and the remaining state are not modelled.
class ShapeObj {
public:
    const ResShape* GetResource() const { return m_pRes; }

    const ResShape* m_pRes;  // 0
    u8 _8_flags;  // 8

private:
    u8 _9[0xd - 0x9];

public:
    u8 m_d;  // d
    u8 _e;
    u8 m_f;  // f

private:
    u8 _10[0x20 - 0x10];

public:
    gfx::Buffer* mBuffers;  // 20

private:
    u8 _28[0x38 - 0x28];

public:
    // Bound sphere of the shape. gsys::ModelRenderUnitNW::getBoundSphereImpl reads this pointer
    // and copies the center and radius out of it.
    Sphere* mBoundSphere;  // 38

private:
    // ModelRenderUnitNW::getShape 0x7100c09d28 indexes this array with a 0x68-byte stride.
    u8 _40[0x68 - 0x40];
};
static_assert(offsetof(ShapeObj, _8_flags) == 0x8);
static_assert(offsetof(ShapeObj, mBuffers) == 0x20);
static_assert(offsetof(ShapeObj, mBoundSphere) == 0x38);
static_assert(sizeof(ShapeObj) == 0x68);

// TODO
class ModelObj {
public:
    SkeletonObj* GetSkeleton() const { return m_Skeleton; }

    // 0x710132eaf4 / 0x710132eafc: forward to the skeleton object.
    void CalculateWorld(const nn::util::neon::MatrixRowMajor4x3fType& mtx);
    void CalculateSkeleton(int index);

    // Inline-only in the original; name is a guess. ModelNW::getName
    // 0x7100c06bc0 and ModelRig 0x7100c8338c read this actual resource.
    const ResModel* GetResource() const { return m_ResModel; }

    s32 GetNumShapes() const { return m_NumShapes; }
    // ModelRenderUnitNW::getShape 0x7100c09d28 indexes the 0x68-byte shape array.
    ShapeObj* GetShape(int index) { return &m_Shapes[index]; }
    const ShapeObj* GetShape(int index) const { return &m_Shapes[index]; }
    // Graphics 0x7100f2e0f4: the material count (+0x32) and array (+0x48).
    s32 GetNumMaterials() const { return m_NumMaterials; }
    MaterialObj* GetMaterial(int index) { return &m_Materials[index]; }
    const MaterialObj* GetMaterial(int index) const { return &m_Materials[index]; }

    // inline-only in the original; name is a guess: +58 user pointer in ModelNW Bone/MaterialVisibilityCallback c04134/c04164.
    void* GetUserPtr() { return m_UserData; }

    s32 get_8c() const { return _8c; }

    // Inline-only in the original; name is a guess. gsys::ModelNW::getViewNum 0x7100c07b88
    // returns the byte at +0x18.
    u8 GetNumViews() const { return _18; }

    // Inline-only; names are ours. gsys::ModelRenderUnitNW::isVisible 0x7100c09d74 tests bit
    // (ResShape +0x5e) of the word array at +0x10, then bit (ResShape +0x60) of the array at +0x8.
    bool IsBoneVisible(int index) const { return m_pBoneVisArray[index >> 5] & (1 << (index & 0x1f)); }
    bool IsMatVisible(int index) const { return m_pMatVisArray[index >> 5] & (1 << (index & 0x1f)); }

private:
    struct InitializeArgument;

    bool Initialize(const InitializeArgument& arg, void* buffer, size_t bufferSize);

    const ResModel* m_ResModel;
    u32* m_pMatVisArray;
    u32* m_pBoneVisArray;
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
