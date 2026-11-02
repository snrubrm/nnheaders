#pragma once

#include <nn/types.h>

namespace nn {
namespace vfx {

class System;
class CallbackSet;

// Partial emitter model: the complete extent is unknown. The set traverses the link
// and updates the callback pointer.
class Emitter {
    friend class EmitterSet;
    u8 _0[0x78];
    Emitter* mNext;  // +0x78
    u8 _80[0x390 - 0x80];
    CallbackSet* mCustomActionCallbackSet;  // +0x390
};

// Partial model. The create id at +0x2c is compared by
// xlink2::OneEmitterMgr::releaseOneEmitterInstance 0x7100bce19c; the setters below are the ones
// whose bodies the game calls directly.
class EmitterSet {
public:
    // 0x7100acaaf4
    void Kill(bool immediate);
    Emitter* GetAliveEmitter(int index) const;
    void OverwriteCustomActionCallbackSet(CallbackSet* callbacks);

    int GetCreateId() const { return m_CreateId; }

    // Inline-only in the original; name is a guess. System's three kill paths
    // test this byte at ACC9DC, ACCA9C and ACCB2C.
    bool IsActiveMaybe() const { return _c != 0; }
    // Inline-only in the original; names are guesses. KillEmitterSetGroup and
    // RemoveEmitterSetList both traverse +0x1c0 and clear the +0xf byte.
    EmitterSet* GetNextMaybe() const { return _1c0; }
    void ClearListFlagMaybe() { _f = 0; }

    // 0x7100aca914: asks every emitter of the set to fade out.
    void Fade();
    // 0x7100aca920 / 0x7100aca930 / 0x7100aca940: the three per-set scale limits.
    void SetEmissionRatioScale(f32 scale);
    void SetEmissionIntervalScale(f32 scale);
    void SetParticleLifeScale(f32 scale);
    // 0x7100acab08: the infinity flag of the emitter resource at +0x228.
    u8 IsHaveInfinityEmitter() const;

private:
    friend class System;

    u8 _0[0x9];
    bool mFade;                       // +0x9
    u8 _a[2];
    u8 _c;
    u8 _d[2];
    u8 _f;
    u8 _10[4];
    s32 _14;
    u8 _18[0x20 - 0x18];
    System* mSystem;                   // +0x20
    u8 _28[0x2c - 0x28];
    int m_CreateId;                    // +0x2c
    u8 _30[0x50 - 0x30];
    f32 mEmissionRatioScale;          // +0x50
    f32 mEmissionIntervalScale;       // +0x54
    f32 mParticleLifeScale;           // +0x58
    u8 _5c[0x150 - 0x5c];
    Emitter* mFirstEmitter;           // +0x150
    u8 _158[0x1c0 - 0x158];
    EmitterSet* _1c0;
    EmitterSet* _1c8;
    u8 _1d0[0x228 - 0x1d0];
    const u8* mResourceMaybe;         // +0x228, the infinity flag is at +0x2a
};

}  // namespace vfx
}  // namespace nn
