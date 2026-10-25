/**
 * @file BasicSound.h
 * @brief A basic sound.
 */

#pragma once

#include <nn/atk/AuxBus.h>
#include <nn/font/font_Util.h>
#include <nn/atk/SoundPlayer.h>
#include <nn/types.h>

namespace nn {
namespace atk {
class SoundActor;
class SoundHandle;
class SoundPlayer;

enum MixMode {

};

namespace detail {
class PlayerHeap;
class ExternalSoundPlayer;
class SoundPlayerHandle;

class BasicSound {
public:
    struct AmbientInfo;

    NN_RUNTIME_TYPEINFO_BASE()

    BasicSound();
    virtual ~BasicSound();

    virtual void Initialize();
    virtual void Finalize();
    virtual bool IsPrepared() const = 0;
    virtual bool IsAttachedTempSpecialHandle() = 0;
    virtual void DetachTempSpecialHandle() = 0;
    // Returns the concrete sound's own player handle (StreamSound answers with its handle at
    // +0x2c0). The base only ever holds the slot.
    virtual SoundPlayerHandle* GetBasicSoundPlayerHandle() = 0;
    virtual void OnUpdatePlayerPriority();
    virtual void UpdateMoveValue();
    virtual void OnUpdateParam();

    void SetPriority(s32, s32);
    void GetPriority(s32*, s32*) const;
    void ClearIsFinalizedForCannotAllocatedResourceFlag();
    void SetId(u32 newID);
    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();
    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();
    void StartPrepared();
    void Stop(s32);
    void SetPlayerPriority(s32);
    void ForceStop();
    void Pause(bool, s32);
    void Pause(bool, s32, nn::atk::PauseMode);
    void Mute(bool, s32);
    void SetAutoStopCounter(s32);
    void FadeIn(s32);
    bool IsPause() const;
    bool IsMute() const;
    void Update();
    void UpdateParam();
    void CalculateVolume() const;
    f32 CalculatePitch() const;
    f32 CalculateLpfFrequency() const;
    u32 CalculateOutLineFlag() const;
    void CalculateBiquadFilter(s32*, f32*) const;
    void AttachPlayerHeap(nn::atk::detail::PlayerHeap*);
    void DetachPlayerHeap(nn::atk::detail::PlayerHeap*);
    void AttachSoundPlayer(nn::atk::SoundPlayer*);
    void DetachSoundPlayer(nn::atk::SoundPlayer*);
    void AttachSoundActor(nn::atk::SoundActor*);
    void DetachSoundActor(nn::atk::SoundActor*);
    void AttachExternalSoundPlayer(nn::atk::detail::ExternalSoundPlayer*);
    void DetachExternalSoundPlayer(nn::atk::detail::ExternalSoundPlayer*);
    u32 GetRemainingFadeFrames() const;
    u32 GetRemainingPauseFadeFrames() const;
    u32 GetRemainingMuteFadeFrames() const;
    void SetInitialVolume(f32 vol);
    f32 GetInitialVolume() const;
    void SetVolume(f32, s32);
    s32 GetVolume() const;
    void SetPitch(f32);
    f32 GetPitch() const;
    void SetLpfFreq(f32);
    f32 GetLpfFreq() const;
    void SetBiquadFilter(s32, f32);
    void GetBiquadFilter(s32*, f32*) const;
    void SetOutputLine(u32);
    u32 GetOutputLine() const;
    void ResetOutputLine();
    void SetMixMode(nn::atk::MixMode);
    nn::atk::MixMode GetMixMode();
    void SetPan(f32);
    f32 GetPan() const;
    void SetSurroundPan(f32);
    f32 GetSurroundPan() const;
    void SetMainSend(f32);
    f32 GetMainSend() const;
    void SetFxSend(nn::atk::AuxBus, f32);

    u64* _8;                                 // nn::atk::detail::PlayerHeap*
    nn::atk::SoundHandle* mGeneralHandle;    // _10
    u64* _18;                                // nn::atk::SoundHandle*
    nn::atk::SoundPlayer* mSoundPlayer;      // _20
    u64* _28;                                // nn::atk::SoundActor*
    u64* _30;                                // nn::atk::detail::ExternalSoundPlayer*
    u64* _38;                                // nn::atk::SoundArchive*
    u8 _40[0xB4 - 0x40];
    s32 _B4;  // a per-frame counter compared with _B8
    s32 _B8;
    u8 _BC[0xD4 - 0xBC];
    s32 _D4;
    s32 _D8;
    u8 mPreparedFlag;  // _DC, StartPrepared stores 1
    u8 _DD[0xE8 - 0xDD];
    u8 mPlayerPriority;     // _E8
    u8 mBiquadFilterIndex;  // _E9
    u8 _EA[0xEC - 0xEA];
    u32 mState;  // _EC, the destructor writes 3
    u8 _F0;
    u8 mPauseState;  // _F1, 1 and 2 count as paused
    u8 _F2[0x100 - 0xF2];
    u32 mID;             // _100
    u32 _104;
    f32 mInitialVolume;  // _108
    f32 mPitch;          // _10C
    f32 mLpfFreq;        // _110
    f32 mBiquadFilterFreq;  // _114
    u32 mOutputLine;        // _118
    f32 mPrevVolume;        // _11C
    f32 mVolume;            // _120
    s32 mVolumeRampTime;    // _124, the two ramp counters compare signed
    s32 mVolumeRampFrame;   // _128
    nn::atk::MixMode mMixMode;  // _12C
    u32 _130;
    u32 _134;
    f32 mMainSend;          // _138
    f32 mFxSend[4];         // _13C, indexed by nn::atk::AuxBus
    u8 _14C[0x158 - 0x14C];
    f32 mOutputVol;  // _158
    u8 _15C[0x190 - 0x15C];
    f32 mOutputPan;          // _190
    f32 mOutputSurroundPan;  // _194
    f32 mOutputMainSend;     // _198
    f32 mOutputFxSend;       // _19C

    static u64 g_LastInstanceId;
};
}  // namespace detail
}  // namespace atk
}  // namespace nn
