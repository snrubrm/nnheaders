#pragma once

#include <nn/types.h>
#include <nn/util/util_BinTypes.h>

namespace nn::g3d {

// Inline-only accessors; names are ours. gsys::ModelRenderUnitNW::getSubMeshNum 0x7100c0a8f0 reads the
// u16 at +0x34 of the 0x38-byte mesh records.
class ResMesh {
public:
    u16 GetSubMeshCount() const { return m_SubMeshCount; }

    u8 _0[0x2c];
    // Read and divided by 3 by gsys::ModelNW::countPolygonNum 0x7100c054bc, so this is the
    // index count of the mesh's first primitive. Name is a guess.
    u32 mIndexCount;
    u8 _30[0x34 - 0x30];
    u16 m_SubMeshCount;
    u8 _36[0x38 - 0x36];
};
static_assert(sizeof(ResMesh) == 0x38);

// Model initialization C6DD78 advances the SDK shape array by 0x70.
class ResShape {
public:
    // Inline-only in the original; name is a guess. Independent C6F3CC and
    // 1200AC0 read this binary string for display-list labels and callback selection.
    const char* GetName() const { return pName.Get()->GetData(); }

    // Inline-only in the original; name is ours. The tera shader binding functions
    // (0x71011413b4, 0x7101141b28, 0x710114251c, 0x7101143c1c, 0x7101144ef4,
    // 0x71011456f0) load a halfword at +0x5e and compare it with an attribute index.
    u16 get_5e() const { return m_5e; }

    // Inline-only; names are ours. gsys::ModelRenderUnitNW::getMeshNum 0x7100c0a8d0 reads the byte at
    // +0x67 and getSubMeshNum 0x7100c0a8f0 indexes the mesh array at +0x20.
    u16 get_60() const { return m_60; }
    // gsys::G3dResShapeEx::setResShape 0x7100c6f580 stores its address at +0x50.
    void set_50(void* value) { m_50 = value; }
    u8 GetMeshCount() const { return m_MeshCount; }
    const ResMesh* GetMesh(int index) const { return &m_pMeshArray[index]; }

    u8 _0[0x10];
    nn::util::BinPtrToString pName;
    u8 _18[0x20 - 0x18];
    ResMesh* m_pMeshArray;
    u8 _28[0x50 - 0x28];
    void* m_50;
    u8 _58[0x5e - 0x58];
    u16 m_5e;
    // See gsys::ModelRenderUnitNW::isVisible (material visibility bit index).
    u16 m_60;
    u8 _62[0x67 - 0x62];
    u8 m_MeshCount;
    u8 _68[0x70 - 0x68];
};
static_assert(sizeof(ResShape) == 0x70);
}  // namespace nn::g3d
