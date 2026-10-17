#pragma once

#include <nn/types.h>

namespace nn {
namespace atk {

enum StreamRegionCallbackResult : int;

/// The parameter of the stream region callback. The game callbacks (0x7101002374) read the
/// region index in the first word; no other field is observable.
struct StreamRegionCallbackParam {
    s32 mRegion;
};

class SoundArchive {
public:
    struct SequenceSoundInfo;

    const char* GetItemLabel(u32 id) const;
    u32 GetItemId(const char* label) const;
};

class SoundActor  // Inherits SoundStartable, size: 0x7C
{
public:
    virtual ~SoundActor();
    u8 data[0x7C - 0x4];
};

}  // namespace atk
}  // namespace nn
