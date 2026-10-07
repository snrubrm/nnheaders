#pragma once

#include <nn/types.h>

namespace nn::g3d {

class AnimFrameCtrl {
public:
    using PlayPolicy = float (*)(float, float, float, void*);
    static float PlayLoop(float frame, float start, float end, void* user_data);

private:
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

protected:
    AnimFrameCtrl* mFrameCtrl;
    AnimFrameCtrl mDefaultFrameCtrl;
    u8 _30[0x18];  // Evaluation context, not yet modeled.
    void* mResultBuffer;
    void* mBuffer;
};

static_assert(sizeof(AnimFrameCtrl) == 0x20);
static_assert(sizeof(AnimObj) == 0x58);

}  // namespace nn::g3d
