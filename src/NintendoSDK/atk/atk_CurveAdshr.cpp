#include <nn/atk/detail/CurveAdshr.h>

namespace nn::atk::detail {

// Native external read-only sustain table, addressed through GOT25a4690.
// The name is a placeholder; all 128 signed-short entries precede sAttackTable.
extern const s16 sUnk_7101ECB2E8[128] = {
    -723, -722, -721, -651, -601, -562, -530, -503, -480, -460, -442, -425, -410, -396, -383, -371,
    -360, -349, -339, -330, -321, -313, -305, -297, -289, -282, -276, -269, -263, -257, -251, -245,
    -239, -234, -229, -224, -219, -214, -210, -205, -201, -196, -192, -188, -184, -180, -176, -173,
    -169, -165, -162, -158, -155, -152, -149, -145, -142, -139, -136, -133, -130, -127, -125, -122,
    -119, -116, -114, -111, -109, -106, -103, -101, -99, -96, -94, -91, -89, -87, -85, -82,
    -80, -78, -76, -74, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50,
    -49, -47, -45, -43, -42, -40, -38, -36, -35, -33, -31, -30, -28, -27, -25, -23,
    -22, -20, -19, -17, -16, -14, -13, -11, -10, -8, -7, -6, -4, -3, -1, 0,
};


namespace {
// Native 128-float table at 1ecb3e8; the independent sequence caller bounds
// attack to 0..127. The following waveform table starts at 1ecb5e8.
const f32 sAttackTable[128] = {
    0.99921751f, 0.998432577f, 0.997645199f, 0.996855319f, 0.996062875f, 0.995267928f, 0.994470417f, 0.993670404f,
    0.992867708f, 0.992062509f, 0.991254628f, 0.990444124f, 0.989630878f, 0.988815129f, 0.987996519f, 0.987175226f,
    0.986351192f, 0.985524416f, 0.984694898f, 0.983862519f, 0.983027279f, 0.982189298f, 0.981348276f, 0.980504513f,
    0.979657829f, 0.978808105f, 0.97795552f, 0.977099895f, 0.976241291f, 0.975379705f, 0.974515021f, 0.973647177f,
    0.972776294f, 0.971902311f, 0.971025109f, 0.970144808f, 0.969261229f, 0.968374372f, 0.967484415f, 0.966591001f,
    0.965694427f, 0.964794397f, 0.963891029f, 0.962984204f, 0.962073982f, 0.961160421f, 0.960243285f, 0.959322572f,
    0.958398402f, 0.957470596f, 0.956539214f, 0.955604196f, 0.954665482f, 0.953723073f, 0.952776909f, 0.95182699f,
    0.950873196f, 0.949915707f, 0.948954225f, 0.947988808f, 0.947019517f, 0.946046174f, 0.945068896f, 0.944087505f,
    0.943102002f, 0.942112386f, 0.941118598f, 0.940120578f, 0.939118385f, 0.938111782f, 0.937100887f, 0.936085582f,
    0.935065925f, 0.934041679f, 0.933013082f, 0.931979775f, 0.930941999f, 0.929899514f, 0.92885232f, 0.927800417f,
    0.926743627f, 0.925682127f, 0.924615622f, 0.923544228f, 0.922467828f, 0.921386421f, 0.920299828f, 0.919208109f,
    0.918111205f, 0.917009115f, 0.915901601f, 0.914788723f, 0.913670301f, 0.912546515f, 0.911417127f, 0.910282075f,
    0.909141421f, 0.907994926f, 0.906842709f, 0.905684471f, 0.904520392f, 0.903350174f, 0.902173996f, 0.900991619f,
    0.899802923f, 0.898608029f, 0.897406578f, 0.896198809f, 0.894984424f, 0.890059888f, 0.882462204f, 0.875924706f,
    0.869186103f, 0.863640606f, 0.853578806f, 0.843018889f, 0.82861352f, 0.814909875f, 0.800217211f, 0.778066278f,
    0.755474985f, 0.724212527f, 0.682823896f, 0.632916927f, 0.559213519f, 0.455141097f, 0.329876989f, 0.0f,
};
}  // namespace


// NON_MATCHING: constant stores are grouped differently.
CurveAdshr::CurveAdshr() {
    Initialize(-90.4f);
}

// NON_MATCHING: default-control stores are grouped differently.
void CurveAdshr::Initialize(f32 value) {
    mDecayRate = 65535.0f;
    mReleaseRate = 65535.0f;
    mAttackRate = 0.0f;
    mHoldTime = 0;
    mSustain = 127;
    Reset(value);
}

void CurveAdshr::Reset(f32 value) {
    mValue = value * 10.0f;
    mState = 0;
}

f32 CurveAdshr::GetValue() const {
    if (mState == 0 && mAttackRate == 0.0f)
        return 0.0f;
    return mValue / 10.0f;
}

// NON_MATCHING: the two special-case blocks are ordered differently.
void CurveAdshr::SetDecay(int decay) {
    switch (decay) {
    case 127:
        mDecayRate = 65535.0f;
        return;
    case 126:
        mDecayRate = 24.0f;
        return;
    default:
        break;
    }
    f32 rate;
    if (decay < 50)
        rate = f32(decay * 2 + 1) / 128.0f;
    else
        rate = 60.0f / f32(126 - decay);
    mDecayRate = rate / 5.0f;
}

// NON_MATCHING: the two special-case blocks are ordered differently.
void CurveAdshr::SetRelease(int release) {
    switch (release) {
    case 127:
        mReleaseRate = 65535.0f;
        return;
    case 126:
        mReleaseRate = 24.0f;
        return;
    default:
        break;
    }
    f32 rate;
    if (release < 50)
        rate = f32(release * 2 + 1) / 128.0f;
    else
        rate = 60.0f / f32(126 - release);
    mReleaseRate = rate / 5.0f;
}

void CurveAdshr::SetAttack(int attack) {
    mAttackRate = sAttackTable[attack];
}

void CurveAdshr::SetHold(int hold) {
    const int time = hold + 1;
    mHoldTime = u16(time * time / 4);
}

void CurveAdshr::SetSustain(int sustain) {
    mSustain = u8(sustain);
}

void CurveAdshr::Update(int milliseconds) {
    switch (mState) {
    case 0:
        while (milliseconds-- > 0) {
            mValue *= mAttackRate;
            if (mValue > -0.03125f) {
                mState = 1;
                mValue = 0.0f;
                mRemainingHoldTime = mHoldTime;
                return;
            }
        }
        return;
    case 1: {
        if (mRemainingHoldTime > milliseconds) {
            mRemainingHoldTime -= milliseconds;
            break;
        }
        milliseconds -= mRemainingHoldTime;
        mRemainingHoldTime = 0;
        mState = 2;
        // Continue into decay for the time left after hold.
    }
    case 2: {
        const f32 sustain = static_cast<f32>(sUnk_7101ECB2E8[mSustain]);
        mValue -= mDecayRate * static_cast<f32>(milliseconds);
        if (mValue < sustain) {
            mValue = sustain;
            mState = 3;
        }
        break;
    }
    case 4:
        mValue -= mReleaseRate * static_cast<f32>(milliseconds);
        break;
    default:
        break;
    }
}

}  // namespace nn::atk::detail
