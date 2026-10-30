#pragma once

#include <nn/atk/detail/StreamSoundPrefetchFile.h>

namespace nn::atk::detail {

class StreamSoundPrefetchFileReader {
public:
    struct PrefetchDataInfo {
        u32 _0;
        u32 _4;
        const void* mSampleAddress;
    };

    StreamSoundPrefetchFileReader();
    void Initialize(const void* file);
    bool ReadStreamSoundInfo(StreamSoundFile::StreamSoundInfo* info) const;
    bool ReadPrefetchDataInfo(PrefetchDataInfo* info, int index) const;

private:
    const StreamSoundPrefetchFile::FileHeader* mFile;
    const StreamSoundFile::InfoBlockBody* mInfo;
    const StreamSoundPrefetchFile::PrefetchDataBlockBody* mPrefetchData;
};
static_assert(sizeof(StreamSoundPrefetchFileReader) == 0x18);

}  // namespace nn::atk::detail
