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
    // Inline-only, names guessed: advanced-wave Prepare, Finalize and frame updates
    // repeatedly test or write the prepared and paused state bytes.
    bool IsPrepared() const { return _30 != 0; }
    void SetPrepared(bool prepared) { _30 = prepared; }
    bool IsPaused() const { return _32 != 0; }
    // Inline-only, name guessed: stream Start, Stop and Update repeatedly test +0x31.
    bool IsStarted() const { return _31 != 0; }
    // Inline-only, name guessed: sequence, wave and advanced-wave Start write +0x31.
    void SetStarted(bool started) { _31 = started; }
    // Inline-only, name guessed: sequence, wave and advanced-wave Pause write +0x32.
    void SetPaused(bool paused) { _32 = paused; }
    // Inline-only, name guessed: wave FinishPlayer, Finalize and Update write +0x33.
    void SetFinished(bool finished) { _33 = finished; }

private:
    nn::os::Event mEvent;  // +0x8
    u8 _30;
    u8 _31;
    u8 _32;
    u8 _33;
    u8 _34[0x44 - 0x34];
    f32 mBiquadFrequency;  // +0x44
    u8 mBiquadType;        // +0x48
    u8 _49[0xb0 - 0x49];
};
static_assert(sizeof(BasicSoundPlayer) == 0xb0);

}  // namespace nn::atk::detail::driver
