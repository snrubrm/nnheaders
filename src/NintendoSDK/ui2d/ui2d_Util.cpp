#include <nn/ui2d/Util.h>
#include <nn/ui2d/AnimTransform.h>
#include <nn/util/util_BytePtr.h>
#include <cstring>

namespace nn::ui2d {

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
