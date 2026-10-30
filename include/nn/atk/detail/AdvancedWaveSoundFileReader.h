#pragma once

#include <nn/atk/detail/AdvancedWaveSoundFile.h>

namespace nn::atk::detail {

struct AdvancedWaveSoundTrackInfoSet {
    struct ClipInfo {
        u32 _0;
        u32 _4;
        u32 _8;
        u32 _c;
        u32 _10;
        u8 _14;
        u8 _15;
        u8 _16[2];
    };
    struct TrackInfo {
        s32 mClipCount;
        ClipInfo mClips[10];
    };
    s32 mTrackCount;
    TrackInfo mTracks[4];
};
static_assert(sizeof(AdvancedWaveSoundTrackInfoSet::TrackInfo) == 0xf4);
// AdvancedWaveSoundPlayer::Prepare stores this at +0xcc and its next field at +0x4a0.
static_assert(sizeof(AdvancedWaveSoundTrackInfoSet) == 0x3d4);

class AdvancedWaveSoundFileReader {
public:
    explicit AdvancedWaveSoundFileReader(const void* file);
    bool ReadWaveSoundTrackInfoSet(AdvancedWaveSoundTrackInfoSet* info);

private:
    const AdvancedWaveSoundFile::InfoBlockBody* mInfo;
};

}  // namespace nn::atk::detail
