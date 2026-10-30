#pragma once

#include <type_traits>
#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/ui2d/Material.h>
#include <nn/util/MathTypes.h>

namespace nn::ui2d {

class AnimTransform;
class DrawInfo;
class Group;
class Layout;
class Pane;
class Material;
struct ResHermiteKey;
class ResourceTextureInfo;
class ShaderInfo;
struct ResExtUserDataList;
struct ResExtUserData;

namespace detail {
void CalculateQuad(DrawInfo&, Material::ConstantBufferForVertexShader*, const util::Float2&, const Size&);
void CalculateQuad(DrawInfo&, Material&, const util::Float2&, const Size&);
void CalculateQuadWithTexCoords(DrawInfo&, Material&, const util::Float2&, const Size&, s32,
                                const util::Float2 (*)[4]);
void DrawQuad(gfx::CommandBuffer&, DrawInfo&, Material&);
}

bool LoadTexture(ResourceTextureInfo*, gfx::Device*, const void*);
void LoadArchiveShader(ShaderInfo*, gfx::Device*, void*, const void*, gfx::MemoryPool*, s64, u64);
void FreeArchiveShader(gfx::Device*, ShaderInfo*);
void ConvertBlendsToArchiveShaderName(char*, s32, s32);
bool ConvertArchiveShaderNameToBlends(s32*, s32*, const char*);
s32 SearchShaderVariationIndexFromTable(const void*, s32, s32);
void BindAnimation(AnimTransform*, Group*, bool);
void UnbindAnimation(AnimTransform*, Group*);
bool IsContain(const Pane*, const util::Float2&);
Pane* FindHitPane(Pane*, const util::Float2&);
const Pane* FindHitPane(const Pane*, const util::Float2&);
Pane* FindHitPane(Layout*, const util::Float2&);
const Pane* FindHitPane(const Layout*, const util::Float2&);
Pane* GetNextPane(Pane*);
Pane* ClonePaneTree(const Pane*, gfx::Device*);
Pane* ClonePaneTreeWithPartsLayout(const Pane*, Layout*, gfx::Device*, Layout*);
bool ComparePaneTreeTest(const Pane*, const Pane*);
void DrawNullAndBoundingPane(gfx::CommandBuffer&, DrawInfo&, Material&, const Pane*,
                             const util::Unorm8x4&, const util::Unorm8x4&);
f32 GetHermiteCurveValue(float, const ResHermiteKey*, s32);
const ResExtUserData* GetExtUserData(const ResExtUserDataList*, const char*);
u64 GetAlignedBufferSize(gfx::Device*, gfx::GpuAccess, u64);
void SetDefaultShaderId(Material*, s32);

// Type check used on panes. T is the result pointer type (e.g. eui::BoundingEx*) and U the
// static pane type (e.g. nn::ui2d::Pane). Like nn::font::DynamicCast, the target type's
// static info is evaluated before the null test. The original instantiation with
// T = eui::BoundingEx* and U = nn::ui2d::Pane is 0x71009300e8.
template <typename T, typename U>
T DynamicCast(U* object) {
    using ClassType = typename std::remove_pointer<T>::type;
    const font::detail::RuntimeTypeInfo* type = ClassType::GetRuntimeTypeInfoStatic();
    if (object && object->GetRuntimeTypeInfo()->IsDerivedFrom(type))
        return static_cast<T>(object);
    return nullptr;
}

}  // namespace nn::ui2d
