#include <nn/atk/EffectAux.h>

namespace nn::atk {

EffectAux::~EffectAux() {}

bool EffectAux::Initialize() {
    return true;
}

void EffectAux::Finalize() {}

void EffectAux::OnChangeOutputMode() {}

void EffectAux::SetEffectBuffer(void* buffer, u64 size) {
    mBuffer = buffer;
    mBufferSize = size;
}

bool EffectAux::IsRemovable() const {
    return nn::audio::IsAuxRemovable(&mAux);
}

}  // namespace nn::atk

