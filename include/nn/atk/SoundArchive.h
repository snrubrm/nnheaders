/**
 * @file SoundArchive.h
 * @brief Sound archive (the bank of sounds of a game) and its two implementations.
 */

#pragma once

#include <nn/types.h>

namespace nn::atk {

namespace detail {
class SoundArchiveFileReader;
namespace fnd {
class FileStream;
}

/// TODO: only the header of the archive file (0x40 bytes) and the info block's reference
/// tables are declared.
struct SoundArchiveFile {
    struct FileHeader {
        u32 GetInfoBlockSize() const;
        u32 GetStringBlockSize() const;

        u8 _0[0x40];
    };

    /// The 8-byte header that precedes every block body (signature, size).
    struct BlockHeader {
        u32 mSignature;
        u32 mSize;
    };

    /// The string block; only the id lookup the reader forwards to is declared.
    struct StringBlockBody {
        // The native signature takes a section enum; section 1 is the item-name table.
        s32 GetItemIdImpl(s32 section, const char* name) const;
        const char* GetString(u32 string_id) const;
    };

    struct StringBlock {
        BlockHeader mHeader;
        StringBlockBody mBody;
    };

    /// A sound record. Only the type accessor the reader forwards to is declared.
    struct SoundInfo {
        u32 GetSoundType() const;
    };

    /// The info block. Each reference table starts with the record count, which is what the
    /// reader's count getters read; the records themselves are not modelled.
    struct InfoBlockBody {
        const SoundInfo* GetSoundInfo(u32 sound_id) const;
        const u32* GetSoundInfoReferenceTable() const;
        const u32* GetPlayerInfoReferenceTable() const;
        const u32* GetSoundGroupInfoReferenceTable() const;
        const u32* GetGroupInfoReferenceTable() const;
        const u32* GetWaveArchiveInfoReferenceTable() const;
        const u32* GetFileInfoReferenceTable() const;
        u32 GetItemFileId(u32 item_id) const;
        u32 GetItemPrefetchFileId(u32 item_id) const;
    };

    struct InfoBlock {
        BlockHeader mHeader;
        InfoBlockBody mBody;
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

    /// The player limits of the archive (aal::StreamingViewer::initialize 0x7100b7d730 reads the stream counts).
    struct SoundArchivePlayerInfo {
        s32 sequenceSoundCount;
        s32 sequenceTrackCount;
        s32 streamSoundCount;
        s32 streamTrackCount;
        s32 streamChannelCount;
        s32 waveSoundCount;
        s32 waveTrackCount;
        s32 streamBufferTimes;
        bool isAdvancedWaveSoundEnabled;
    };

    // Only the record pointer is needed by the wrappers below; the record layout is not modelled.
    struct AdvancedWaveSoundInfo;

    bool IsAvailable() const;
    u32 GetSoundCount() const;
    u32 GetPlayerCount() const;
    u32 GetGroupCount() const;
    u32 GetWaveArchiveCount() const;
    u32 detail_GetFileCount() const;
    u32 GetItemFileId(u32 item_id) const;
    u32 GetItemPrefetchFileId(u32 item_id) const;
    u32 GetSoundType(u32 sound_id) const;
    bool detail_ReadAdvancedWaveSoundInfo(u32 sound_id, AdvancedWaveSoundInfo* info) const;
    const u32* detail_GetWaveArchiveIdTable(u32 wave_archive_id) const;
    bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* info) const;
    const char* GetItemLabel(u32 item_id) const;
    u32 GetItemId(const char* label) const;

protected:
    // IsAvailable tests it and every accessor above forwards to it.
    detail::SoundArchiveFileReader* mFileReader;  // +0x8
    u8 _10[0x290];
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
