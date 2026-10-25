#include <nn/atk/detail/BasicSound.h>

#include <algorithm>

#include <nn/atk/SoundHandle.h>
#include <nn/atk/SoundPlayer.h>

namespace nn {
namespace atk {
namespace detail {

void BasicSound::SetId(u32 id) {
    mID = id;
}

bool BasicSound::IsAttachedGeneralHandle() {
    return mGeneralHandle != nullptr;
}

void BasicSound::DetachGeneralHandle() {
    mGeneralHandle->DetachSound();
}

void BasicSound::StartPrepared() {
    mPreparedFlag = 1;
}

void BasicSound::AttachSoundPlayer(SoundPlayer* player) {
    mSoundPlayer = player;
}

void BasicSound::DetachSoundPlayer(SoundPlayer*) {
    mSoundPlayer = nullptr;
}

void BasicSound::AttachSoundActor(SoundActor* actor) {
    _28 = reinterpret_cast<u64*>(actor);
}

void BasicSound::AttachExternalSoundPlayer(ExternalSoundPlayer* player) {
    _30 = reinterpret_cast<u64*>(player);
}

void BasicSound::DetachExternalSoundPlayer(ExternalSoundPlayer*) {
    _30 = nullptr;
}

void BasicSound::SetInitialVolume(f32 volume) {
    if (volume < 0.0f)
        volume = 0.0f;
    mInitialVolume = volume;
}

void BasicSound::SetPitch(f32 pitch) {
    mPitch = pitch;
}

void BasicSound::SetLpfFreq(f32 freq) {
    mLpfFreq = freq;
}

void BasicSound::SetBiquadFilter(s32 type, f32 freq) {
    mBiquadFilterIndex = type;
    mBiquadFilterFreq = freq;
}

void BasicSound::SetOutputLine(u32 line) {
    mOutputLine = line;
}

void BasicSound::ResetOutputLine() {
    mOutputLine = mSoundPlayer->mDefaultOutputLine;
}

void BasicSound::SetMixMode(MixMode mode) {
    mMixMode = mode;
}

void BasicSound::SetMainSend(f32 send) {
    mMainSend = send;
}

void BasicSound::SetFxSend(AuxBus bus, f32 send) {
    mFxSend[bus] = send;
}

bool BasicSound::IsPause() const {
    return mPauseState == 1 || mPauseState == 2;
}

void BasicSound::OnUpdatePlayerPriority() {
}

void BasicSound::OnUpdateParam() {
}

}  // namespace detail

void SoundPlayer::detail_SetPlayableSoundLimit(s32 limit) {
    mPlayableSoundCount = limit;
}

}  // namespace atk
}  // namespace nn
