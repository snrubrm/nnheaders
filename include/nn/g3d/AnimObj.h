#pragma once

#include <nn/types.h>

namespace nn::g3d {

class AnimFrameCache;

class AnimFrameCtrl {
public:
    using PlayPolicy = float (*)(float, float, float, void*);

    // 0x710132b6d0 inlines this as the AnimObj's default controller.
    AnimFrameCtrl()
        : mFrame(0.0f), mStartFrame(0.0f), mEndFrame(0.0f), mStep(1.0f),
          mPlayPolicy(&PlayOneTime), mUserData(nullptr)
    {
    }

    static float PlayLoop(float frame, float start, float end, void* user_data);
    // The other play policy: the frame is clamped to [start, end] and does not loop.
    static float PlayOneTime(float frame, float start, float end, void* user_data);

    // Inlined by BoneBlendState 0x7101257934: the play policy maps the frame.
    void SetFrame(float frame) { mFrame = mPlayPolicy(frame, mStartFrame, mEndFrame, mUserData); }

    // inline-only in the original; name is a guess. Swarm::Unit 0x71002da3a0 and
    // ModelAnimation::setMaterialAnmFrame 0x7100bfe384 read the current frame.
    float GetFrame() const { return mFrame; }

private:
    friend class AnimObj;

    float mFrame;
    float mStartFrame;
    float mEndFrame;
    float mStep;
    PlayPolicy mPlayPolicy;
    void* mUserData;
};

// RTTI at 0x710252c9b8 names AnimObj. Its constructor (0x710132b6d0)
// places the default frame controller at +0x10 and its pointer at +8.
class AnimObj {
public:
    // CalculateImpl checks bit 30; the partial mask update checks bit 31.
    // These descriptive enumerator names reconstruct the two skip operations.
    enum BindFlag {
        BindFlag_Enable = 0,
        BindFlag_SkipCalculate = 1,
        BindFlag_SkipApply = 2,
        BindFlag_Disable = 3,
    };

    virtual ~AnimObj();
    virtual void ClearResult() = 0;
    virtual void Calculate() = 0;

    // 0x710132b6d0: points mFrameCtrl at the default controller, whose frame and
    // bounds are zero, step 1.0 and play policy PlayOneTime.
    AnimObj();

    // 0x710132b708: resets the default frame controller. The play policy is
    // PlayLoop when `flag` is set and PlayOneTime otherwise.
    void ResetFrameCtrl(int frame, bool flag);

    // inline-only in the original; name is a guess. Swarm::Unit 0x71002da3a0 and
    // ModelAnimation::setMaterialAnmFrame 0x7100bfe384 read this controller.
    AnimFrameCtrl* GetFrameCtrl() const { return mFrameCtrl; }

protected:
    AnimFrameCtrl* mFrameCtrl;
    AnimFrameCtrl mDefaultFrameCtrl;
    u8 _30[0x18];  // Evaluation context, not yet modeled.
    void* mResultBuffer;
    void* mBuffer;
};

// The class name comes from the original symbol. Only the first 0x10 bytes
// are recovered: Initialize (0x710132b6bc) stores the frame cache pointer at
// +0, the entry count at +8 and a zero word at +0xc. The extent of the
// original class is not known.
class AnimContext {
public:
    void Initialize(AnimFrameCache* cache, int count);

private:
    AnimFrameCache* _0;
    s32 _8;
    s32 _c;
};

static_assert(sizeof(AnimFrameCtrl) == 0x20);
static_assert(sizeof(AnimObj) == 0x58);
static_assert(sizeof(AnimContext) == 0x10);

}  // namespace nn::g3d
