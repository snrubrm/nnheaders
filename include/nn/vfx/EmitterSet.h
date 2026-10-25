#pragma once

#include <nn/types.h>

namespace nn {
namespace vfx {

// Partial model. The create id at +0x2c is compared by
// xlink2::OneEmitterMgr::releaseOneEmitterInstance 0x7100bce19c; the setters below are the ones
// whose bodies the game calls directly.
class EmitterSet {
public:
    // 0x7100acaaf4
    void Kill(bool immediate);

    int GetCreateId() const { return m_CreateId; }

    // 0x7100aca914: asks every emitter of the set to fade out.
    void Fade();
    // 0x7100aca920 / 0x7100aca930 / 0x7100aca940: the three per-set scale limits.
    void SetEmissionRatioScale(f32 scale);
    void SetEmissionIntervalScale(f32 scale);
    void SetParticleLifeScale(f32 scale);
    // 0x7100acab08: the infinity flag of the emitter resource at +0x228.
    u8 IsHaveInfinityEmitter() const;

private:
    u8 _0[0x9];
    bool mFade;                       // +0x9
    u8 _a[0x2c - 0xa];
    int m_CreateId;                    // +0x2c
    u8 _30[0x50 - 0x30];
    f32 mEmissionRatioScale;          // +0x50
    f32 mEmissionIntervalScale;       // +0x54
    f32 mParticleLifeScale;           // +0x58
    u8 _5c[0x228 - 0x5c];
    const u8* mResourceMaybe;         // +0x228, the infinity flag is at +0x2a
};

}  // namespace vfx
}  // namespace nn
