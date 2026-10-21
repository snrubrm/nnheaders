#pragma once

#include <nn/types.h>

namespace gsys {
class ModelAnimation;
class ModelNW;
}  // namespace gsys

namespace nn::g3d {

// The blend-weight callback interface. The release build has exactly one implementation
// (ksys::as::ASList, vtable 0x710250ff98): five virtual functions and the destructor last.
// Only slot 2 is ever called: gsys::ModelAnimation::Exec (0x7100bff26c) loads the implementation
// from ModelAnimation+0x90 and calls slot 2 with the animation object, the value of
// ModelAnimation+0xb8 (the model unit), the u16 at ModelAnimation+0xb6 (the bone index) and the
// argument. The implementation forwards the unit to a helper that calls its virtual method at
// vtable+0x40, which is gsys::ModelNW::getBoneName, so the parameter types are the game's
// animation wrapper and model and are forward declared here.
// Slots 0 / 1 / 3 / 4 have empty bodies in the only implementation and no caller in the release
// build; their meaning is unknown, so they keep placeholder names.
class ICalculateBlendWeightCallback {
public:
    // The blend producers (0x13349a8 / 0x1334ad8) construct this index/weight
    // pair; gsys::ModelAnimation::Exec (0xbff26c) forwards it unchanged.
    struct CallbackArg {
        s32 bone_index;
        f32 weight;
    };

    virtual void unk0() = 0;
    virtual void unk1() = 0;
    virtual void calculateBlendWeight(gsys::ModelAnimation* animation, gsys::ModelNW* unit,
                                      s32 bone_index, CallbackArg& arg) = 0;
    virtual void unk3() = 0;
    virtual void unk4() = 0;
    virtual ~ICalculateBlendWeightCallback() = default;
};
static_assert(sizeof(ICalculateBlendWeightCallback::CallbackArg) == 8);

}  // namespace nn::g3d
