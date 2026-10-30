#pragma once

#include <nn/atk/detail/StreamSoundFile.h>

namespace nn::atk::detail {

class StreamSoundFileReader {
public:
    struct TrackInfo {
        u8 _0;
        u8 _1;
        u8 _2;
        u8 _3;
        u8 mChannelCount;
        u8 mChannelIndices[2];
    };

    StreamSoundFileReader();
    void Initialize(const void* file);
    bool IsValidFileHeader(const void* file);
    bool IsTrackInfoAvailable() const;
    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* info) const;
    bool ReadStreamTrackInfo(TrackInfo* info, int index) const;

private:
    const StreamSoundFile::FileHeader* mFile;
    const StreamSoundFile::InfoBlockBody* mInfo;
};
static_assert(sizeof(StreamSoundFileReader) == 0x10);

}  // namespace nn::atk::detail
