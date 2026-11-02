#include <nn/vfx/EmitterSet.h>
#include <nn/vfx/System.h>

#include <algorithm>

namespace nn {
namespace vfx {

void EmitterSet::Kill(bool immediate) {
    mSystem->KillEmitterSet(this, immediate);
}

// NON_MATCHING: the compiler emits a different loop and countdown sequence.
Emitter* EmitterSet::GetAliveEmitter(int index) const {
    Emitter* emitter = mFirstEmitter;
    unsigned int remaining = index;
    while (emitter && remaining--)
        emitter = emitter->mNext;
    return emitter;
}

void EmitterSet::OverwriteCustomActionCallbackSet(CallbackSet* callbacks) {
    for (Emitter* emitter = mFirstEmitter; emitter; emitter = emitter->mNext)
        emitter->mCustomActionCallbackSet = callbacks;
}

void EmitterSet::SetManualParticleEmissionWithParticleCount(int count) {
    mManualParticleEmission = true;
    mManualParticleCount = count;
    for (Emitter* emitter = mFirstEmitter; emitter; emitter = emitter->mNext) {
        if (emitter->_90) {
            emitter->mParticleCount = mManualParticleCount;
            emitter->ResourceUpdate(mDevice, emitter->_508);
        }
    }
}

void EmitterSet::Fade() {
    mFade = true;
}

void EmitterSet::SetEmissionRatioScale(f32 scale) {
    if (scale > 1.0f)
        scale = 1.0f;
    mEmissionRatioScale = scale;
}

void EmitterSet::SetEmissionIntervalScale(f32 scale) {
    if (scale < 1.0f)
        scale = 1.0f;
    mEmissionIntervalScale = scale;
}

void EmitterSet::SetParticleLifeScale(f32 scale) {
    if (scale > 1.0f)
        scale = 1.0f;
    mParticleLifeScale = scale;
}

u8 EmitterSet::IsHaveInfinityEmitter() const {
    return mResourceMaybe[0x2a];
}

}  // namespace vfx
}  // namespace nn
