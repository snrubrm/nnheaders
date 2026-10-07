/**
 * @file SoundArchive.h
 * @brief Sound archive (the bank of sounds of a game) and its two implementations.
 */

#pragma once

#include <nn/types.h>

namespace nn::atk {

namespace detail {
namespace fnd {
class FileStream;
}

/// TODO: only the header of the archive file (0x40 bytes) is declared.
struct SoundArchiveFile {
    struct FileHeader {
        u32 GetInfoBlockSize() const;
        u32 GetStringBlockSize() const;

        u8 _0[0x40];
    };
};
}  // namespace detail

/// TODO: only what sead::AudioSoundDataMgrCafe and its archives use is declared (the class is 0x2a0 bytes).
class SoundArchive {
public:
    virtual ~SoundArchive();

    virtual const void* detail_GetFileAddress(u32 file_id) const = 0;
    virtual size_t detail_GetRequiredStreamBufferSize() const = 0;
    virtual void FileAccessBegin() const {}
    virtual void FileAccessEnd() const {}
    virtual bool IsAddon() const { return false; }
    virtual detail::fnd::FileStream* OpenStream(void* buffer, size_t buffer_size, s64 offset, size_t size) const = 0;
    virtual detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size, const char* ext_path,
                                                   const char* name, size_t size) const = 0;

    bool IsAvailable() const;
    u32 GetSoundCount() const;
    const char* GetItemLabel(u32 item_id) const;
    u32 GetItemId(const char* label) const;

protected:
    u8 _8[0x298];
};
static_assert(sizeof(SoundArchive) == 0x2a0);

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
    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size, const char* ext_path, const char* name,
                                           size_t size) const override;

    detail::SoundArchiveFile::FileHeader mFileHeader;
    u8 _2e0[0x371 - 0x2e0];
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
    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size, const char* ext_path, const char* name,
                                           size_t size) const override;

private:
    u8 _2a0[0x2f8 - 0x2a0];
};
static_assert(sizeof(MemorySoundArchive) == 0x2f8);

}  // namespace nn::atk
