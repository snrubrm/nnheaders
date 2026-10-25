/**
 * @file SoundArchive.h
 * @brief Sound archive (the bank of sounds of a game) and its two implementations.
 */

#pragma once

#include <nn/types.h>
#include <nn/util/util_BytePtr.h>

namespace nn::atk {

namespace detail {
class SoundArchiveFileReader;
namespace fnd {
class FileStream;
}

/// TODO: only the header of the archive file (0x40 bytes) and the info block's reference
/// tables are declared.
struct SoundArchiveFile {
    /// One of the three block descriptors the file header holds; the signature names the block
    /// (0x2000 string, 0x2001 info, 0x2002 file) and offset / size locate it in the file.
    struct BlockDescriptor {
        u16 mSignature;  // +0
        u16 _2;
        u32 mOffset;  // +4
        u32 mSize;    // +8
    };

    struct FileHeader {
        /// The file identifier, "FSAR" in memory order.
        static constexpr u32 cSignature = 0x52415346;
        /// The oldest and newest version words the reader accepts.
        static constexpr u32 cMinVersion = 0x10000;
        static constexpr u32 cMaxVersion = 0x20300;

        u32 GetSignature() const { return mSignature; }
        u32 GetVersion() const { return mVersion; }
        u32 GetInfoBlockSize() const;
        u32 GetStringBlockSize() const;
        u32 GetStringBlockOffset() const;
        u32 GetInfoBlockOffset() const;
        u32 GetFileBlockOffset() const;

    private:
        const BlockDescriptor* findBlock(u16 signature) const;

        u32 mSignature;  // +0, "FSAR"
        u32 mFileSize;   // +4
        u32 mVersion;    // +8
        u8 _c[8];
        BlockDescriptor mBlocks[3];  // +0x14
    };
    static_assert(sizeof(FileHeader) == 0x38);

    /// The 8-byte header that precedes every block body (signature, size).
    struct BlockHeader {
        u32 mSignature;
        u32 mSize;
    };

    /// The string block; only the id lookup the reader forwards to is declared.
    struct StringBlockBody {
        /// The sections whose names the block can look up; only the item section is used here.
        enum Sections {
            Sections_Item = 1,
        };

        s32 GetItemIdImpl(Sections section, const char* name) const;
        const char* GetString(u32 string_id) const;

    private:
        u8 _0[4];
        s32 mStringTableOffset;  // +4, relative to this
    };

    struct StringBlock {
        BlockHeader mHeader;
        StringBlockBody mBody;
    };

    /// A sound record. The signature at +0xc names the sound type and +0x10 is the offset of
    /// the type-specific sub-info that follows the record body.
    struct StreamSoundTrackInfo;
    struct SequenceSoundInfo;
    struct AdvancedWaveSoundInfo;
    struct PlayerInfo;

    /// A stream sound record. The track info table and the extension block are both found by a
    /// signature at a fixed offset and located by a relative offset next to it.
    struct StreamSoundInfo {
        const StreamSoundTrackInfo* GetTrackInfoTable() const;
        const char* GetStreamSoundExtension() const;

    private:
        u8 _0[4];
        u16 mTrackInfoSignature;  // +0x4
        u8 _6[2];
        s32 mTrackInfoOffset;  // +0x8
        u8 _c[8];
        s32 mSendValueOffset;  // +0x14
        u16 mExtensionSignature;  // +0x18
        u8 _1a[2];
        s32 mExtensionOffset;  // +0x1c, -1 when there is no extension
    };

    /// A wave sound record; the optional values are present when bit 0 of the flag byte is set.
    struct WaveSoundInfo {
        u32 GetChannelPriority() const;
        u32 GetIsReleasePriorityFix() const;
        u32 GetPrefetchFileId() const { return mPrefetchFileId; }

    private:
        u8 _0[8];
        u8 mFlags;  // +0x8
        u8 _9[3];
        u32 mChannelPriority;  // +0xc, the release-priority-fix flag sits in its upper bits
        u8 _10[0x10];
        u32 mPrefetchFileId;  // +0x20
    };

    /// A sequence sound record; the start offset is optional (bit 0 of the flag byte at +0xc).
    struct SequenceSoundInfo {
        u32 GetStartOffset() const;

    private:
        u8 _0[0xc];
        u8 mFlags;  // +0xc
        u8 _d[3];
        u32 mStartOffset;  // +0x10
    };

    /// A file record: the signature names the internal (0x220c) or external (0x220d) file info
    /// that the offset at +4 points at.
    struct InternalFileInfo;
    struct ExternalFileInfo;

    struct FileInfo {
        const InternalFileInfo* GetInternalFileInfo() const;
        const ExternalFileInfo* GetExternalFileInfo() const;

    private:
        u16 mSignature;  // +0
        u8 _2[2];
        s32 mFileInfoOffset;  // +4
    };

    /// The file id every item record starts with, and the sound id a sound group starts with.
    struct BankInfo {
        u32 mItemFileId;  // +0
    };

    struct SoundGroupInfo {
        u32 mSoundId;  // +0
    };

    struct GroupInfo {
        u32 mItemFileId;  // +0
    };

    struct WaveArchiveInfo {
        u32 mItemFileId;  // +0
    };

    struct SoundInfo {
        const StreamSoundInfo* GetStreamSoundInfo() const;
        const WaveSoundInfo* GetWaveSoundInfo() const;
        const SequenceSoundInfo* GetSequenceSoundInfo() const;
        const AdvancedWaveSoundInfo* GetAdvancedWaveSoundInfo() const;
        u32 GetSoundType() const;

        u32 mItemFileId;  // +0

        u16 GetSignature() const { return mSignature; }

    private:
        u8 _4[0x8];
        u16 mSignature;     // +0xc
        s32 mSubInfoOffset; // +0x10, relative to this
    };

    /// The info block. Each of the seven tables is described by a {u32, s32} pair (the second word
    /// is the table's offset from the block body) and the table itself starts with its record
    /// count, which is what the reader's count getters read; the records themselves are not
    /// modelled.
    struct InfoBlockBody {
        const SoundInfo* GetSoundInfo(u32 sound_id) const;
        const BankInfo* GetBankInfo(u32 bank_id) const;
        const PlayerInfo* GetPlayerInfo(u32 player_id) const;
        const SoundGroupInfo* GetSoundGroupInfo(u32 sound_group_id) const;
        const GroupInfo* GetGroupInfo(u32 group_id) const;
        const WaveArchiveInfo* GetWaveArchiveInfo(u32 wave_archive_id) const;
        const FileInfo* GetFileInfo(u32 file_id) const;
        const u32* GetSoundInfoReferenceTable() const;
        const u32* GetPlayerInfoReferenceTable() const;
        const u32* GetSoundGroupInfoReferenceTable() const;
        const u32* GetGroupInfoReferenceTable() const;
        const u32* GetWaveArchiveInfoReferenceTable() const;
        const u32* GetFileInfoReferenceTable() const;
        u32 GetItemFileId(u32 item_id) const;
        u32 GetItemPrefetchFileId(u32 item_id) const;
        s32 GetItemStringId(u32 item_id) const;

    private:
        u8 _0[4];
        s32 mSoundInfoTableOffset;         // +0x4
        u8 _8[4];
        s32 mSoundGroupInfoTableOffset;    // +0xc
        u8 _10[4];
        s32 mBankInfoTableOffset;          // +0x14
        u8 _18[4];
        s32 mWaveArchiveInfoTableOffset;   // +0x1c
        u8 _20[4];
        s32 mGroupInfoTableOffset;         // +0x24
        u8 _28[4];
        s32 mPlayerInfoTableOffset;        // +0x2c
        u8 _30[4];
        s32 mFileInfoTableOffset;          // +0x34
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
    void Initialize(detail::SoundArchiveFileReader* reader);
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
    u8 _10[0x288];
    // +0x298, added to a file record's offset when a stream is opened. The original's data ends
    // at 0x2a0, so the class has a second word here that nothing we model reads.
    u32 mFileBlockOffset;
    u32 _29c;
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
    detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size, const char* ext_path, const char* name,
                                           size_t size) const override;

private:
    u8 _2a0[0x2f8 - 0x2a0];
};
static_assert(sizeof(MemorySoundArchive) == 0x2f8);

}  // namespace nn::atk
