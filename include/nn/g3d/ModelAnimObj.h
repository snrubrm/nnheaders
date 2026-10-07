#pragma once

#include <nn/g3d/AnimObj.h>

namespace nn::g3d {

// Initialize (0x710132b58c), BindAll and the bind-flag methods establish
// this table. Each entry holds two 15-bit indices and a two-bit bind flag.
class AnimBindTable {
public:
    void sub_710132B58C(u32* entries, s32 capacity);
    void BindAll(const u16* indices);

    u32* mEntries;
    u16 mFlags;
    u16 mCapacity;
    u16 mAnimCount;
    u16 mTargetCount;
};

// RTTI at 0x710252ca20 identifies AnimObj as the direct base.
class ModelAnimObj : public AnimObj {
public:
    void SetBindFlagImpl(s32 index, BindFlag flag);

protected:
    AnimBindTable mBindTable;
};

static_assert(sizeof(AnimBindTable) == 0x10);
static_assert(sizeof(ModelAnimObj) == 0x68);

}  // namespace nn::g3d
