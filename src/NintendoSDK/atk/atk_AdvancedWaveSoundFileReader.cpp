#include <nn/atk/detail/AdvancedWaveSoundFileReader.h>
#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

// NON_MATCHING: signature and version checks have a different branch layout.
AdvancedWaveSoundFileReader::AdvancedWaveSoundFileReader(const void* file) {
    const auto* header = static_cast<const nn::util::BinaryFileHeader*>(file);
    const auto& signature = header->signature._str;
    if (signature[0] != 'B' || signature[1] != 'A' || signature[2] != 'W' ||
        signature[3] != 'S' || signature[4] != 'D' || signature[5] != ' ' ||
        signature[6] != ' ' || signature[7] != ' ')
        return;
    if (header->version.major != 1 || header->version.minor != 0 || header->version.micro != 0)
        return;
    const auto* block = header->GetFirstBlock();
    if (!block || block->signature._str[0] != 'I' || block->signature._str[1] != 'N' ||
        block->signature._str[2] != 'F' || block->signature._str[3] != 'O')
        return;
    mInfo = nn::util::ConstBytePtr(block, sizeof(nn::util::BinaryBlockHeader))
                .Get<AdvancedWaveSoundFile::InfoBlockBody>();
}

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
