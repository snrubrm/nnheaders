#pragma once

#include <nn/os/os_Event.h>
#include <nn/types.h>

namespace nn::atk::detail::driver {

// Partial base: native player constructors place the event at +0x8 and derived players
// implement the three pure playback operations.
class BasicSoundPlayer {
public:
    BasicSoundPlayer();
    virtual ~BasicSoundPlayer();

    virtual void Initialize();
    virtual void Finalize();
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Pause(bool pause) = 0;

    void SetBiquadFilter(int type, float frequency);

protected:
    // Inline-only, name guessed: stream Start, Stop and Update repeatedly test +0x31.
    bool IsStarted() const { return _31 != 0; }

private:
    nn::os::Event mEvent;  // +0x8
    u8 _30;
    u8 _31;
    u8 _32[0x44 - 0x32];
    f32 mBiquadFrequency;  // +0x44
    u8 mBiquadType;        // +0x48
    u8 _49[0xb0 - 0x49];
};
static_assert(sizeof(BasicSoundPlayer) == 0xb0);

}  // namespace nn::atk::detail::driver
