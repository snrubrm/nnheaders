#pragma once

#include <nn/audio.h>
#include <nn/atk/OutputMode.h>
#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {

// Partial model: the virtual interface, list node and effect buffer state are modelled.
class EffectAux {
public:
    EffectAux();
    struct UpdateSamplesArg {
        s32 samplesPerFrame;
        s32 sampleRate;
        s32 audioFrameCount;
        s32 channelCount;
        s32 sampleCount;
        OutputMode outputMode;
    };

    static_assert(sizeof(UpdateSamplesArg) == 0x18);

    virtual ~EffectAux();
    virtual bool Initialize();
    virtual void Finalize();
    virtual void OnChangeOutputMode();
    virtual void UpdateSamples(s32* samples, const UpdateSamplesArg& arg) = 0;
    void SetEffectBuffer(void* buffer, u64 size);
    size_t GetRequiredMemSize(const nn::audio::AudioRendererParameter& parameter) const;
    bool SetChannelCount(s32 count);
    bool SetAudioFrameCount(s32 count);
    void SetEnabled(bool enabled);
    bool AddEffect(nn::audio::AudioRendererConfig* config,
                   const nn::audio::AudioRendererParameter& parameter, nn::audio::FinalMixType* mix);
    bool AddEffect(nn::audio::AudioRendererConfig* config,
                   const nn::audio::AudioRendererParameter& parameter, nn::audio::SubMixType* mix);
    void SetEffectInputOutput(const s8* input, const s8*, int, int);
    void RemoveEffect(nn::audio::AudioRendererConfig* config, nn::audio::FinalMixType* mix);
    void RemoveEffect(nn::audio::AudioRendererConfig* config, nn::audio::SubMixType* mix);
    void Update();
    // 0x710135bde8
    bool IsRemovable() const;

private:
    nn::util::IntrusiveListNode mNode;  // +0x8
    /* 0x18 */ nn::audio::AuxType mAux;
    s32 mAudioFrameCount;
    s32 mChannelCount;
    bool mIsAdded;
    bool mIsEnabled;
    u8 _2a[0x30 - 0x2a];
    /* 0x30 */ void* mBuffer;
    /* 0x38 */ u64 mBufferSize;
    s32* mSampleBuffer;  // +0x40
    u32 mChannelIndices[6];
};
static_assert(sizeof(EffectAux) == 0x60);

}  // namespace nn::atk
