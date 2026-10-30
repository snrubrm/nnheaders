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
        struct SectionReference {
            u32 _0;
            s32 mOffset;
        };
        SectionReference mSections[2];  // offsets relative to this block body
    };

    struct StringBlock {
        BlockHeader mHeader;
        StringBlockBody mBody;
    };

    /// A sound record. The signature at +0xc names the sound type and +0x10 is the offset of
    /// the type-specific sub-info that follows the record body.
    struct StreamSoundTrackInfo;
    struct SequenceSoundInfo;
    struct AdvancedWaveSoundInfo { u32 _0; };
    struct PlayerInfo;
    struct SoundArchivePlayerInfo {
        u16 mSequenceSoundCount;
        u16 mSequenceTrackCount;
        u16 mStreamSoundCount;
        u16 mStreamTrackCount;
        u16 mStreamChannelCount;
        u16 mWaveSoundCount;
        u16 mWaveTrackCount;
        u8 mStreamBufferTimes;
        u8 mFlags;
    };
    struct StreamSoundExtension {
        u8 _0;
        u8 _1;
        u8 _2[2];
        u32 _4;
        u32 _8;
    };

    /// A stream sound record. The track info table and the extension block are both found by a
    /// signature at a fixed offset and located by a relative offset next to it.
    struct StreamSoundInfo {
        const StreamSoundTrackInfo* GetTrackInfoTable() const;
        const StreamSoundExtension* GetStreamSoundExtension() const;
        const u8* GetSendValue() const;

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
        u8 GetIsReleasePriorityFix() const;
        u32 GetPrefetchFileId() const { return mPrefetchFileId; }

        u32 _0;
        u32 _4;

    private:
        u8 mFlags;  // +0x8
        u8 _9[3];
        u32 mChannelPriority;  // +0xc, the release-priority-fix flag sits in its upper bits
        u8 _10[0x10];
        u32 mPrefetchFileId;  // +0x20
    };

    /// A sequence sound record; the start offset is optional (bit 0 of the flag byte at +0xc).
    struct SequenceSoundInfo {
        u32 GetStartOffset() const;
        // 0x7101340e78: the bank id table (count, ids) at the offset at +4; four ids, -1 past
        // the count.
        void GetBankIds(u32* bankIds) const;
        // 0x7101340f0c / 0x7101340f2c: the optional value for bit 1 of the option word at +0xc.
        u32 GetChannelPriority() const;
        bool IsReleasePriorityFix() const;

    private:
        u8 _0[4];
        s32 mBankIdTableOffset;  // +0x4

    public:
        u32 _8;

    private:
        u32 mOptionFlags;  // +0xc, followed by the present optional values
        u32 mStartOffset;  // +0x10
    };

    /// A file record: the signature names the internal (0x220c) or external (0x220d) file info
    /// that the offset at +4 points at.
    struct InternalFileInfo {
        u32 _0;
        s32 mOffset;
        u32 mSize;
    };
    struct ExternalFileInfo;

    struct FileInfo {
        u32 GetFileLocationType() const;
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
        u32 _4;
        s32 mWaveArchiveTableOffset;  // +0x8, relative to this record
    };

    struct SoundGroupInfo {
        u32 mSoundId;  // +0
        u32 _4;
        u16 _8;
        u16 _a;
        s32 mTableOffset;  // +0xc, relative to this record
        u32 _10;
        s32 mWaveArchiveReferenceOffset;  // +0x14, relative to this record
    };

    struct GroupInfo {
        u32 mItemFileId;  // +0
    };

    /// 0x7101340f78: the wave count is the optional value for bit 1 of the option flags at +8;
    /// the value for bit 0 (when present) comes first.
    struct WaveArchiveInfo {
        u32 GetWaveCount() const;

        u32 mItemFileId;  // +0
        u8 _4;
        u8 _5[3];
        u32 mOptionFlags;  // +0x8, followed by the present optional values
    };

    /// 0x7101340f58: the heap size is the optional value for bit 1 of the option flags at +4.
    struct PlayerInfo {
        u32 GetPlayerHeapSize() const;

        u32 mPlayerSoundLimit;  // +0
        u32 mOptionFlags;       // +0x4, followed by the present optional values
    };

    struct SoundInfo {
        // 0x7101340ce8..0x7101340d84: optional values of the option parameter at +0x14.
        u32 GetPanMode() const;
        u32 GetPanCurve() const;
        u32 GetPlayerPriority() const;
        u8 GetActorPlayerId() const;
        bool IsFrontBypass() const;

        const StreamSoundInfo* GetStreamSoundInfo() const;
        const WaveSoundInfo* GetWaveSoundInfo() const;
        const SequenceSoundInfo* GetSequenceSoundInfo() const;
        const AdvancedWaveSoundInfo* GetAdvancedWaveSoundInfo() const;
        u32 GetSoundType() const;

        u32 mItemFileId;  // +0
        u32 _4;
        u8 _8;
        u8 _9;
        u8 _a[2];

        u16 GetSignature() const { return mSignature; }

    private:
        u16 mSignature;     // +0xc
        s32 mSubInfoOffset; // +0x10, relative to this
        u32 mOptionFlags;   // +0x14, followed by the present optional values
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
        const SoundArchivePlayerInfo* GetSoundArchivePlayerInfo() const;

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
        u8 _38[4];
        s32 mSoundArchivePlayerInfoOffset;  // +0x3c
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
    // The mangled symbol substitutes the first void* for the fourth parameter, so the
    // original declaration's fourth parameter is a void* (not a string).
    virtual detail::fnd::FileStream* OpenExtStream(void* buffer, size_t buffer_size,
                                                   const char* ext_path, void* name,
                                                   size_t size) const = 0;

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

    struct GroupInfo {
        u32 mFileId;
        u32 mFileSize;
    };
    struct FileInfo {
        u32 mFileSize;
        s32 mFileOffset;
        const void* _8;
    };
    struct StreamSoundInfo2 {
        bool _0;
        u32 _4;
        u32 _8;
    };
    struct WaveArchiveInfo {
        u32 mFileId;
        u32 mWaveCount;
        u8 _8;
    };
    struct SequenceSoundInfo {
        u32 mStartOffset;
        u32 mBankIds[4];
        u32 _14;
        u8 mChannelPriority;
        bool mReleasePriorityFix;
    };
    struct WaveSoundInfo {
        u32 _0;
        u32 _4;
        u8 mChannelPriority;
        bool mReleasePriorityFix;
    };
    struct BankInfo {
        u32 mFileId;
    };
    struct PlayerInfo {
        u32 mSoundLimit;
        u32 mHeapSize;
    };
    struct SoundGroupInfo {
        u32 _0;
        u32 _4;
        const void* _8;
    };

    struct SoundInfo {
        u32 mFileId;
        u32 _4;
        u8 mActorPlayerId;
        u8 mPlayerPriority;
        u8 _a;
        u8 _b;
        u32 mPanMode;
        u32 mPanCurve;
        bool mFrontBypass;
    };
    struct AdvancedWaveSoundInfo { u32 _0; };

    bool IsAvailable() const;
    void Initialize(detail::SoundArchiveFileReader* reader);
    void Finalize();
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
    u8 _10[0x18 - 0x10];
    // Finalize 0x7101339f40 stores the constant 47 here after it releases the reader.
    u16 mFileVersion;  // +0x18
    u8 _1a[0x298 - 0x1a];
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
    u8 _2a0[0x2f8 - 0x2a0];
};
static_assert(sizeof(MemorySoundArchive) == 0x2f8);

}  // namespace nn::atk
