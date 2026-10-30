#pragma once

#include <nn/atk/AuxBus.h>
#include <nn/atk/detail/driver/BasicSoundPlayer.h>
#include <nn/atk/detail/driver/SoundThread.h>
#include <nn/types.h>

namespace nn::atk::detail::driver {

// The native update path passes each of the eight 0x80-byte records as a StreamTrack*.
class StreamTrack {
    friend class StreamSoundPlayer;

    u8 _0[0x19];
    u8 mInitialVolume;  // +0x19
    u8 _1a[0x24 - 0x1a];
    f32 mVolume;       // +0x24
    u32 mOutputLine;   // +0x28
    f32 mTvVolume;     // +0x2c
    u8 _30[0x64 - 0x30];
    f32 mTvPan;        // +0x64
    f32 mTvSurroundPan;
    f32 mTvMainSend;
    f32 mTvFxSend[3];
    u8 _7c[4];
};
static_assert(sizeof(StreamTrack) == 0x80);

// Partial player: construction and unmodeled members remain native dependencies.
class StreamSoundPlayer : public BasicSoundPlayer, public SoundThread::PlayerCallback {
public:
    StreamSoundPlayer();
    ~StreamSoundPlayer() override;
    void Initialize() override;
    void Finalize() override;
    void Start() override;
    void Stop() override;
    void Pause(bool pause) override;
    void Update();
    void OnUpdateFrameSoundThread() override;
    void OnUpdateFrameSoundThreadWithAudioFrameFrequency() override;
    void OnShutdownSoundThread() override;

    void SetTrackVolume(u32 track_mask, f32 volume);
    void SetTrackInitialVolume(u32 track_mask, u32 volume);
    void SetTrackOutputLine(u32 track_mask, u32 line);
    void ResetTrackOutputLine(u32 track_mask);
    void SetTrackTvVolume(u32 track_mask, f32 volume);
    void SetTrackTvPan(u32 track_mask, f32 pan);
    void SetTrackTvSurroundPan(u32 track_mask, f32 pan);
    void SetTrackTvMainSend(u32 track_mask, f32 send);
    void SetTrackTvFxSend(u32 track_mask, nn::atk::AuxBus bus, f32 send);

private:
    u8 _c8[0x43c - 0xc8];
    s32 mTrackCount;  // +0x43c; Setup clamps it to eight
    u8 _440[0x10c40 - 0x440];
    StreamTrack mTracks[8];  // +0x10c40
    u32 _11040;
};

}  // namespace nn::atk::detail::driver
