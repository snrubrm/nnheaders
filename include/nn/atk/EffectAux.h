#pragma once

#include <nn/types.h>

namespace nn::atk {

// TODO: incomplete. Only the effect buffer pair at +0x30 is modelled; the renderer state and
// the effect list are not.
class EffectAux {
public:
    EffectAux();
    ~EffectAux();
    bool Initialize();
    void Finalize();
    void OnChangeOutputMode();
    void SetEffectBuffer(void* buffer, u64 size);

private:
    u8 _0[0x30];
    /* 0x30 */ void* mBuffer;
    /* 0x38 */ u64 mBufferSize;
};

}  // namespace nn::atk
