/**
 * @file ResExtUserData.h
 * @brief Extended user data stored in layout resources (named values attached to panes and controls).
 */

#pragma once

#include <nn/types.h>

namespace nn::ui2d {

// Layout of the resource blocks: eui::FindExtUserDataFromList (0x7100bed250) and Pane::FindExtUserDataByName
// (0x7100ab9644) read the u16 count at +8 and walk 0xc-byte entries starting at +0xc; callers read the data
// value at entry + the u32 at +4 (strings, f32 pairs: eui::AdjustToText*, LocalizeReplaceTarget).
// The block header, the type field and the padding follow the NintendoWare layout resource format.
struct ResBlockHeader {
    u32 kind;
    u32 size;
};

enum ExtUserDataType {
    ExtUserDataType_String,
    ExtUserDataType_Int,
    ExtUserDataType_Float,
};

struct ResExtUserData {
    u32 nameStrOffset;
    u32 dataOffset;
    u16 count;
    u8 type;
    u8 padding;

    // Both offsets are relative to the entry itself. A zero name offset means there is no name.
    const char* GetName() const {
        return nameStrOffset != 0 ? reinterpret_cast<const char*>(this) + nameStrOffset : nullptr;
    }

    const char* GetString() const { return reinterpret_cast<const char*>(this) + dataOffset; }
    const s32* GetIntArray() const { return reinterpret_cast<const s32*>(GetString()); }
    const f32* GetFloatArray() const { return reinterpret_cast<const f32*>(GetString()); }

    u16 GetCount() const { return count; }
    ExtUserDataType GetType() const { return static_cast<ExtUserDataType>(type); }
};
static_assert(sizeof(ResExtUserData) == 0xc);

// The entries (ResExtUserData[count]) directly follow this header.
struct ResExtUserDataList {
    ResBlockHeader blockHeader;
    u16 count;
    u8 padding[2];

    u16 GetCount() const { return count; }
    const ResExtUserData* GetArray() const { return reinterpret_cast<const ResExtUserData*>(this + 1); }
};
static_assert(sizeof(ResExtUserDataList) == 0xc);

}  // namespace nn::ui2d
