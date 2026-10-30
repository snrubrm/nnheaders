#pragma once

#include <nn/atk/detail/StreamSoundPrefetchFile.h>

namespace nn::atk::detail {

class StreamSoundPrefetchFileReader {
public:
    StreamSoundPrefetchFileReader();
    void Initialize(const void* file);
    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* info) const;

private:
    const StreamSoundPrefetchFile::FileHeader* mFile;
    const StreamSoundFile::InfoBlockBody* mInfo;
    const StreamSoundPrefetchFile::PrefetchData* mPrefetchData;
};
static_assert(sizeof(StreamSoundPrefetchFileReader) == 0x18);

}  // namespace nn::atk::detail
