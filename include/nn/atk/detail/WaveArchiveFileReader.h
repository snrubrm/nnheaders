#pragma once

#include <nn/atk/detail/WaveArchiveFile.h>

namespace nn::atk::detail {

class WaveArchiveFileReader {
public:
    WaveArchiveFileReader();
    WaveArchiveFileReader(const void* file, bool useFileTable);
    void Initialize(const void* file, bool useFileTable);
    bool HasIndividualLoadTable() const;
    void Finalize();
    void InitializeFileTable();
    const void* GetWaveFile(u32 index) const;
    u32 GetWaveFileSize(u32 index) const;
    u32 GetWaveFileOffsetFromFileHead(u32 index) const;
    const void* SetWaveFile(u32 index, const void* file);

private:
    const WaveArchiveFile::FileHeader* mFile;
    const WaveArchiveFile::InfoBlockBody* mInfo;
    WaveArchiveFile::FileTableEntry* mFileTable;
    bool mInitialized;
};
static_assert(sizeof(WaveArchiveFileReader) == 0x20);

}  // namespace nn::atk::detail
