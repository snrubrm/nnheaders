#include <nn/ui2d/Util.h>
#include <nn/ui2d/AnimTransform.h>
#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_GpuAddress.h>
#include <nn/ui2d/DrawInfo.h>
#include <nn/ui2d/GraphicsResource.h>
#include <nn/ui2d/Material.h>
#include <nn/ui2d/ResourceTextureInfo.h>
#include <nn/ui2d/ShaderInfo.h>
#include <nn/util/util_BytePtr.h>
#include <cstring>

namespace nn::ui2d {

// 0x7100abc670
// NON_MATCHING: pool-offset register allocation and argument setup scheduling differ.
bool LoadTexture(ResourceTextureInfo* info, gfx::Device* device, const void* data) {
    auto* resource = static_cast<gfx::ResTexture*>(const_cast<void*>(data));
    info->mResource = resource;
    gfx::detail::TextureImpl<gfx::DefaultApi>* texture = resource->GetTexture();
    if (!texture || !texture->ToData()->state) {
        auto& texture_data = resource->ToData();
        auto& container = *texture_data.pResTextureContainerData.Get();
        const ptrdiff_t image_offset = container.memoryPoolOffsetBase +
            util::ConstBytePtr(container.pTextureData.Get(), sizeof(util::BinaryBlockHeader))
                .Distance(texture_data.pMipPtrArray.Get()[0].Get());
        texture->Initialize(device, *resource->GetTextureInfo(),
                            container.pCurrentMemoryPool.Get(), image_offset,
                            texture_data.textureDataSize);

        gfx::TextureViewInfo view_info;
        view_info.SetDefault();
        view_info.SetImageDimension(static_cast<gfx::ImageDimension>(texture_data.imageDimension));
        view_info.SetChannelMapping(static_cast<gfx::ChannelMapping>(texture_data.channelMapping[0]),
                                    static_cast<gfx::ChannelMapping>(texture_data.channelMapping[1]),
                                    static_cast<gfx::ChannelMapping>(texture_data.channelMapping[2]),
                                    static_cast<gfx::ChannelMapping>(texture_data.channelMapping[3]));
        view_info.SetImageFormat(resource->GetTextureInfo()->GetImageFormat());
        view_info.SetTexturePtr(resource->GetTexture());
        view_info.EditSubresourceRange().EditArrayRange().SetArrayLength(
            resource->GetTextureInfo()->GetArrayLength());
        view_info.EditSubresourceRange().EditMipRange().SetMipCount(
            resource->GetTextureInfo()->GetMipCount());
        gfx::detail::TextureViewImpl<gfx::DefaultApi>* view = resource->GetTextureView();
        view->Initialize(device, view_info);
    }
    return true;
}

// 0x7100abc98c
// NON_MATCHING: search branch layout and floating-point instruction scheduling differ.
f32 GetHermiteCurveValue(f32 frame, const ResHermiteKey* keys, s32 count) {
    const s32 last = count - 1;
    if (count == 1 || frame <= keys[0].frame)
        return keys[0].value;
    if (keys[last].frame <= frame)
        return keys[last].value;

    s32 low = 0;
    s32 high = last;
    while (low != high - 1 && low != high) {
        const s32 middle = (low + high) / 2;
        if (frame <= keys[middle].frame)
            high = middle;
        else
            low = middle;
    }

    const f32 distance_to_high = frame - keys[high].frame;
    if (distance_to_high > -0.001f && distance_to_high < 0.001f) {
        if (high < last && keys[high].frame == keys[high + 1].frame)
            return keys[high + 1].value;
        return keys[high].value;
    }

    const f32 distance = frame - keys[low].frame;
    const f32 inverse_span = 1.0f / (keys[high].frame - keys[low].frame);
    const f32 square = distance * distance * inverse_span;
    const f32 cube = distance * (inverse_span * square);
    const f32 triple = (inverse_span * square) * 3.0f;
    const f32 twice = inverse_span * cube + inverse_span * cube;
    return keys[low].value * ((twice - triple) + 1.0f) +
           keys[high].value * (triple - twice) +
           keys[low].tangent * (distance + (cube - (square + square))) +
           keys[high].tangent * (cube - square);
}

namespace detail {
// 0x7100ac0214
void CalculateQuad(DrawInfo& draw_info, Material::ConstantBufferForVertexShader* buffer,
                   const util::Float2& position, const Size& size) {
    buffer->size.width = size.width;
    buffer->size.height = size.height;
    buffer->position.x = position.x;
    buffer->position.y = position.y;
    draw_info.sub_7100AC074C(&buffer->matrix);
}

// 0x7100ac023c
void CalculateQuad(DrawInfo& draw_info, Material& material, const util::Float2& position,
                   const Size& size) {
    CalculateQuad(draw_info, material.GetConstantBufferForVertexShader(draw_info), position, size);
}

// 0x7100ac029c
// NON_MATCHING: native unrolls the three coordinate slots; this version retains the loop.
void CalculateQuadWithTexCoords(DrawInfo& draw_info, Material& material,
                                const util::Float2& position, const Size& size, s32 count,
                                const util::Float2 (*coordinates)[4]) {
    auto* buffer = material.GetConstantBufferForVertexShader(draw_info);
    if (count > 0 && coordinates) {
        for (s32 slot = 0; slot < 3; ++slot) {
            const s8 index = draw_info._e8[slot];
            if (index < 0)
                break;
            if (index < count) {
                for (s32 vertex = 0; vertex < 4; ++vertex) {
                    buffer->textureCoordinates[slot][vertex].x = coordinates[index][vertex].x;
                    buffer->textureCoordinates[slot][vertex].y = coordinates[index][vertex].y;
                    buffer->textureCoordinates[slot][vertex].z = 0.0f;
                    buffer->textureCoordinates[slot][vertex].w = 1.0f;
                }
            }
        }
    }
    CalculateQuad(draw_info, buffer, position, size);
}

// 0x7100ac045c
void DrawQuad(gfx::CommandBuffer& command_buffer, DrawInfo& draw_info, Material& material) {
    material.sub_7100AC50CC(&command_buffer);
    draw_info.sub_7100AC0720(&command_buffer);
    material.SetCommandBuffer(command_buffer, draw_info);
    gfx::GpuAddress address;
    draw_info.mGraphicsResource->mBuffer1.GetGpuAddress(&address);
    command_buffer.gfx::detail::CommandBufferImpl<gfx::DefaultApi>::DrawIndexed(
        gfx::PrimitiveTopology_TriangleList, gfx::IndexFormat_Uint16, address, 6, 0);
}
}  // namespace detail

void LoadArchiveShader(ShaderInfo* info, gfx::Device* device, void* shader_file,
                       const void* archive_data, gfx::MemoryPool* memory_pool,
                       s64 memory_pool_offset, u64 memory_pool_size) {
    info->Initialize(device, shader_file, archive_data, memory_pool,
                     memory_pool_offset, memory_pool_size);
}

// 0x7100abc950
void BindAnimation(AnimTransform* transform, Group* group, bool enabled) {
    transform->BindGroup(group);
    transform->SetEnabled(enabled);
}

// 0x7100abc73c
// NON_MATCHING: decimal remainder instruction selection differs.
void ConvertBlendsToArchiveShaderName(char* name, s32 blend, s32 alpha_blend) {
    name[0] = '0' + blend / 100;
    blend %= 100;
    name[1] = '0' + blend / 10;
    name[2] = '0' + blend % 10;
    name[3] = '_';
    name[4] = '0' + alpha_blend / 100;
    alpha_blend %= 100;
    name[5] = '0' + alpha_blend / 10;
    name[6] = '0' + alpha_blend % 10;
    name[7] = '\0';
}

// 0x7100abc7e4
// NON_MATCHING: character-range checks and partial output stores are scheduled differently.
bool ConvertArchiveShaderNameToBlends(s32* blend, s32* alpha_blend, const char* name) {
    if (std::strlen(name) != 7)
        return false;
    if (name[0] < '0' || name[0] > '9')
        return false;
    *blend = (name[0] - '0') * 100;
    if (name[1] < '0' || name[1] > '9')
        return false;
    *blend += (name[1] - '0') * 10;
    if (name[2] < '0' || name[2] > '9')
        return false;
    *blend += name[2] - '0';
    if (name[3] != '_')
        return false;
    if (name[4] < '0' || name[4] > '9')
        return false;
    *alpha_blend = (name[4] - '0') * 100;
    if (name[5] < '0' || name[5] > '9')
        return false;
    *alpha_blend += (name[5] - '0') * 10;
    if (name[6] < '0' || name[6] > '9')
        return false;
    *alpha_blend += name[6] - '0';
    return true;
}

// 0x7100abc900
// NON_MATCHING: loop induction width and the not-found return branches differ.
s32 SearchShaderVariationIndexFromTable(const void* table, s32 blend, s32 alpha_blend) {
    // Serialized table: a u16 count followed by six-byte entries. Each entry
    // stores the two blend values and the variation index.
    const u16* data = util::ConstBytePtr(table).Get<u16>();
    for (s32 i = 0; i < data[0]; ++i) {
        const u16* entry = data + 1 + i * 3;
        if (entry[0] == blend && entry[1] == alpha_blend)
            return entry[2];
    }
    return -1;
}

}  // namespace nn::ui2d
