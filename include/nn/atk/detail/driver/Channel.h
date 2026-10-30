#pragma once

#include <nn/atk/detail/CurveAdshr.h>
#include <nn/atk/detail/CurveLfo.h>
#include <nn/atk/detail/driver/DisposeCallback.h>
#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk::detail::driver {

class MultiVoice;
class SequenceTrack;

// Partial real Channel: opaque spans retain the still-unmodeled members.
class Channel {
public:
    enum ChannelCallbackStatus : int;
    using Callback = void (*)(Channel*, ChannelCallbackStatus, void*);

    Channel();
    ~Channel();
    static void DetachChannel(Channel* channel);
    static void FreeChannel(Channel* channel);
    void SetBiquadFilter(int type, float frequency);
    void Release();
    void NoteOff();
    void UpdateSweep(int milliseconds);
    void SetSweepParam(float value, int duration, bool flag);
    void Stop();
    void CallChannelCallback(ChannelCallbackStatus status);

private:
    friend class SequenceTrack;

    class Disposer : public DisposeCallback {
    public:
        Disposer();
        ~Disposer() override;
        void InvalidateData(const void* begin, const void* end) override;

    private:
        Channel* mOwner;  // +0x18
    };
    static_assert(sizeof(Disposer) == 0x20);

    Disposer mDisposer;       // +0x0
    CurveAdshr mEnvelope;     // +0x20
    CurveLfo mLfo0;           // +0x3c
    CurveLfo mLfo1;           // +0x5c
    CurveLfo mLfo2;           // +0x7c
    CurveLfo mLfo3;           // +0x9c
    u8 _bc[4];
    u8 mPaused;               // +0xc0
    u8 mActive;               // +0xc1
    u8 _c2;
    u8 _c3;
    u8 _c4[2];
    u8 mBiquadType;           // +0xc6
    u8 _c7[0xd4 - 0xc7];
    f32 mBiquadFrequency;     // +0xd4
    u8 _d8[0x130 - 0xd8];
    f32 _130;
    s32 _134;
    s32 _138;
    u8 _13c[0x158 - 0x13c];
    s32 _158;
    u8 _15c[0x170 - 0x15c];
    Callback mCallback;       // +0x170
    void* mCallbackArg;       // +0x178
    MultiVoice* mVoice;       // +0x180
    Channel* mNext;           // +0x188
    u8 _190[0x3e0 - 0x190];
    nn::util::IntrusiveListNode mPoolNode;  // +0x3e0
};

}  // namespace nn::atk::detail::driver
