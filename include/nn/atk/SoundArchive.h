/**
 * @file SoundArchive.h
 * @brief Sound archive and its file-system and memory implementations.
 */
#pragma once

#include <nn/atk/detail/SoundArchiveBase.h>
#include <nn/atk/detail/SoundArchiveFileReader.h>

namespace nn::atk {

/// A sound archive that is read from the file system (0x618 bytes).
class FsSoundArchive : public SoundArchive {
public:
    FsSoundArchive();
    ~FsSoundArchive() override;

    bool Open(const char* path);
    void Close();
    bool LoadHeader(void* buffer, size_t size);
    bool LoadLabelStringData(void* buffer, size_t size);

    const void* detail_GetFileAddress(u32) const override { return nullptr; }
    size_t detail_GetRequiredStreamBufferSize() const override;
    void FileAccessBegin() const override;
    void FileAccessEnd() const override;
    detail::fnd::FileStream* OpenStream(void* buffer, size_t buffer_size, s64 offset, size_t size) const override;
    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size,
                                           const char* ext_path, void* name,
                                           size_t size) const override;

    detail::SoundArchiveFile::FileHeader mFileHeader;
    u8 _2d8[0x371 - 0x2d8];
    bool _371;
    u8 _372[0x618 - 0x372];
};
static_assert(sizeof(FsSoundArchive) == 0x618);

/// A sound archive that is in memory (0x2f8 bytes).
class MemorySoundArchive : public SoundArchive {
public:
    MemorySoundArchive();
    ~MemorySoundArchive() override;

    bool Initialize(const void* data);
    void Finalize();

    const void* detail_GetFileAddress(u32 file_id) const override;
    size_t detail_GetRequiredStreamBufferSize() const override;
    detail::fnd::FileStream* OpenStream(void* buffer, size_t buffer_size, s64 offset, size_t size) const override;
    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size,
                                           const char* ext_path, void* name,
                                           size_t size) const override;

private:
    const void* mData;                       // +0x2a0
    detail::SoundArchiveFileReader mReader;  // +0x2a8
};
static_assert(sizeof(MemorySoundArchive) == 0x2f8);

}  // namespace nn::atk
