#include <nn/atk/detail/AdvancedWaveSoundFileReader.h>

namespace nn::atk::detail {

bool AdvancedWaveSoundFileReader::ReadWaveSoundTrackInfoSet(AdvancedWaveSoundTrackInfoSet* info) {
    info->mTrackCount = *mInfo->GetTrackReferenceTable();
    for (int i = 0; i < info->mTrackCount; ++i) {
        const auto* track = mInfo->GetWaveSoundTrack(i);
        info->mTracks[i].mClipCount = *track->GetClipReferenceTable();
        for (int j = 0; j < info->mTracks[i].mClipCount; ++j) {
            const auto* clip = track->GetWaveSoundClip(j);
            auto& out = info->mTracks[i].mClips[j];
            out._0 = clip->_0;
            out._4 = clip->_4;
            out._8 = clip->_8;
            out._c = clip->_c;
            out._10 = clip->_10;
            out._14 = clip->_14;
            out._15 = clip->_15;
        }
    }
    return true;
}

}  // namespace nn::atk::detail
