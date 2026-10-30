#pragma once

#include <nn/audio.h>
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
    size_t GetRequiredMemSize(const nn::audio::AudioRendererParameter& parameter) const;
    bool SetChannelCount(s32 count);
    bool SetAudioFrameCount(s32 count);
    void SetEnabled(bool enabled);
    void SetEffectInputOutput(const s8* input, const s8*, int, int);
    // 0x710135bde8
    bool IsRemovable() const;

private:
    u8 _0[0x18];
    /* 0x18 */ nn::audio::AuxType mAux;
    s32 mAudioFrameCount;
    s32 mChannelCount;
    bool mIsAdded;
    bool mIsEnabled;
    u8 _2a[0x30 - 0x2a];
    /* 0x30 */ void* mBuffer;
    /* 0x38 */ u64 mBufferSize;
    u8 _40[8];
    u32 mChannelIndices[6];
};

}  // namespace nn::atk
