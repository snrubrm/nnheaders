#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// Partial model of the advanced wave sound resource file. Only the table offset the
// reference-table getter reads is modelled.
class AdvancedWaveSoundFile {
public:
    struct WaveSoundClip;

    struct WaveSoundTrack {
        // 0x71013657f4: an unsigned offset at +4, relative to the track, to a table of clip
        // offsets (count, then u32 offsets relative to the table).
        const u32* GetClipReferenceTable() const;
        // 0x7101365800
        const WaveSoundClip* GetWaveSoundClip(int index) const;

    private:
        u8 _0[4];
        u32 mClipReferenceTableOffset;  // +0x4
    };

    struct InfoBlockBody {
        // 0x71013657bc: a self-relative table offset at +0.
        const u32* GetTrackReferenceTable() const;
        // 0x71013657c8: the table holds a count and u32 offsets relative to the table.
        const WaveSoundTrack* GetWaveSoundTrack(int index) const;

    private:
        u32 mTrackReferenceTableOffset;  // +0x0
    };
};

}  // namespace nn::atk::detail
