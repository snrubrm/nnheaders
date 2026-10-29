#include <nn/atk/detail/AdvancedWaveSoundFile.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {

const u32* AdvancedWaveSoundFile::InfoBlockBody::GetTrackReferenceTable() const {
    return nn::util::ConstBytePtr(this, mTrackReferenceTableOffset).Get<u32>();
}

const AdvancedWaveSoundFile::WaveSoundTrack*
AdvancedWaveSoundFile::InfoBlockBody::GetWaveSoundTrack(int index) const {
    const u32* table = GetTrackReferenceTable();
    if (index >= static_cast<s32>(table[0]))
        return nullptr;
    const u32* items = table + 1;
    return nn::util::ConstBytePtr(table, items[index]).Get<WaveSoundTrack>();
}

const u32* AdvancedWaveSoundFile::WaveSoundTrack::GetClipReferenceTable() const {
    return nn::util::ConstBytePtr(this, mClipReferenceTableOffset).Get<u32>();
}

const AdvancedWaveSoundFile::WaveSoundClip*
AdvancedWaveSoundFile::WaveSoundTrack::GetWaveSoundClip(int index) const {
    const u32* table = GetClipReferenceTable();
    if (index >= static_cast<s32>(table[0]))
        return nullptr;
    const u32* items = table + 1;
    return nn::util::ConstBytePtr(table, items[index]).Get<WaveSoundClip>();
}

}  // namespace nn::atk::detail
