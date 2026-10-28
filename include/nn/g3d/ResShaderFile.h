#pragma once

#include <nn/types.h>
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_BinTypes.h>
#include <nn/util/util_ResDic.h>

namespace nn::g3d {

class ResModel;

// The resource layout remains opaque. Only native static entrypoints are declared.
class ResShaderFile {
public:
    static bool IsValid(const void* data);
    static ResShaderFile* ResCast(void* data);

    // The relocated shader archive at +0x20; gsys::ModelShaderArchive::setBinary 0x7100c3cea0
    // stores this pointer in its archive at +0x30.
    u8 _0[0x20];
    void* mShaderArchive;  // 20
};

// Same resource prefix as ResFile: the relocated file name at +0x20, the model record array at
// +0x28 and its dictionary at +0x30. ResFile derives from this archive (ModelResource stores a
// ResFile* at +0x38 and the Mii part setup calls FindShadingModel on it without an offset), and
// FindShadingModel 0x7100f4d8cc indexes the array with the 0x78 stride of a ResModel.
class ResShaderArchive : public util::BinaryFileHeader {
public:
    ResModel* FindShadingModel(const char* name);

    // Inline-only in the original; name is a guess. 0x710125ac84 reads the binary string through
    // this pointer and adds the two-byte length to reach the characters.
    const char* GetName() const { return pName.Get()->GetData(); }

    nn::util::BinPtrToString pName;  // _20
    ResModel* mModels;               // _28
    util::ResDic* mModelDict;        // _30
};

// The class name comes from the original symbol. Only the four bytes at
// +0x1d..+0x23 are recovered: the dynamic base index, the key index, the
// shift and the mask of the shader option. The extent of the original class
// is not known.
class ResShaderOption {
public:
    u32 ReadStaticKey(const u32* keys) const;
    void WriteStaticKey(u32* keys, int value) const;
    void WriteDynamicKey(u32* keys, int value) const;

private:
    u8 _0[0x1d];
    u8 _1d;   // the base index of a dynamic key
    u8 _1e;   // the key index
    u8 _1f;   // the shift
    u32 _20;  // the mask
};
static_assert(sizeof(ResShaderOption) == 0x24);

}  // namespace nn::g3d
