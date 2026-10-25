#include <nn/vfx/EmitterSet.h>

#include <algorithm>

namespace nn {
namespace vfx {

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
