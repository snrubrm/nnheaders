#pragma once

#include <nn/atk/SampleFormat.h>
#include <nn/atk/detail/Voice.h>
#include <nn/types.h>

namespace nn::atk {

// Partial models: the enumerator sets are not recovered.
enum class PanMode : s32 {};
enum class PanCurve : s32 {};

}  // namespace nn::atk

namespace nn::atk::detail::driver {

// TODO: incomplete. The object starts with two Voice instances (at +0 and +0xd8) and carries the
// format at +0x24c; only those parts are modelled.
class MultiVoice : public Voice {
public:
    SampleFormat GetFormat() const;
    s64 GetCurrentPlayingSample() const;

    // 0x71013572dc
    void Start();
    // 0x7101357490
    void Pause(bool pause);
    // 0x7101357a9c
    void SetVolume(f32 volume);
    // 0x7101357ac4
    void SetPitch(f32 pitch);
    // 0x7101357ae4
    void SetPanMode(nn::atk::PanMode mode);
    // 0x7101357b04
    void SetPanCurve(nn::atk::PanCurve curve);
    // 0x7101357b24
    void SetLpfFreq(f32 frequency);
    // 0x7101357c04
    void SetOutputLine(u32 line);

private:
    u8 _d8[0x1b0 - 0xd8];
    /* 0x1b0 */ s32 mVoiceCount;
    u8 _1b4[0x1c9 - 0x1b4];
    /* 0x1c9 */ bool mStarted;
    u8 _1ca;
    /* 0x1cb */ bool mPaused;
    u8 _1cc[0x1d8 - 0x1cc];
    /* 0x1d8 */ u16 mFlags;
    u8 _1da[0x1dc - 0x1da];
    /* 0x1dc */ f32 mVolume;
    /* 0x1e0 */ f32 mPitch;
    /* 0x1e4 */ nn::atk::PanMode mPanMode;
    /* 0x1e8 */ nn::atk::PanCurve mPanCurve;
    /* 0x1ec */ f32 mLpfFreq;
    u8 _1f0[0x1f8 - 0x1f0];
    /* 0x1f8 */ u32 mOutputLine;
    u8 _1fc[0x24c - 0x1fc];
    /* 0x24c */ SampleFormat mFormat;
};

}  // namespace nn::atk::detail::driver
