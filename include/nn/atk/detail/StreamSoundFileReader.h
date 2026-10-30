#pragma once

#include <nn/atk/detail/StreamSoundFile.h>

namespace nn::atk::detail {

class StreamSoundFileReader {
public:
    StreamSoundFileReader();
    void Initialize(const void* file);
    bool IsValidFileHeader(const void* file);
    bool IsTrackInfoAvailable() const;

private:
    const StreamSoundFile::FileHeader* mFile;
    const StreamSoundFile::InfoBlockBody* mInfo;
};
static_assert(sizeof(StreamSoundFileReader) == 0x10);

}  // namespace nn::atk::detail
