/**
 * @file SoundDataManager.h
 * @brief Sound data management implementation.
 */

#pragma once

#include <nn/atk/SoundArchive.h>
#include <nn/atk/SoundHeap.h>
#include <nn/types.h>

namespace nn {
namespace atk {
namespace detail {

/// TODO: only what SoundDataManager uses is declared.
class SoundArchiveLoader {
public:
    SoundArchiveLoader();
    virtual ~SoundArchiveLoader();

    virtual const void* SetFileAddressToTable(u32 file_id, const void* address) = 0;
    virtual const void* GetFileAddressFromTable(u32 file_id) const = 0;
    virtual const void* GetFileAddressImpl(u32 file_id) const = 0;

    bool IsAvailable() const;
    const void* GetFileAddressFromSoundArchive(u32 file_id) const;
    /// 0x13427fc (declared only)
    bool LoadData(const char* label, SoundHeap* heap, u32 load_flag, u32 unknown);

protected:
    u8 _8[0x230 - 0x18 - 8];
};

/// The first base of SoundDataManager (0x18 bytes, the contents are not known: the constructor sets the two pointers
/// to the address of the first one).
class SoundDataManagerBase {
public:
    virtual ~SoundDataManagerBase();

    void* _8;
    void* _10;
};
}  // namespace detail

/// TODO: only what sead::AudioSoundDataMgrCafe uses is declared (the class is 0x240 bytes).
class SoundDataManager : public detail::SoundDataManagerBase, public detail::SoundArchiveLoader {
public:
    SoundDataManager();
    ~SoundDataManager() override;

    virtual void InvalidateData(const void* start, const void* end);
    const void* SetFileAddressToTable(u32 file_id, const void* address) override;
    const void* GetFileAddressFromTable(u32 file_id) const override;
    const void* GetFileAddressImpl(u32 file_id) const override;
    /// 0x133c1c8 (declared only; the name is not known)
    virtual const void* GetFileTableEntry(u32 file_id) const;

    size_t GetRequiredMemSize(const SoundArchive* archive) const;
    bool Initialize(const SoundArchive* archive, void* buffer, size_t buffer_size);
    void Finalize();
    const void* detail_GetFileAddress(u32 file_id) const;

private:
    void* mFileTable;
    void* _238;
};
static_assert(sizeof(SoundDataManager) == 0x240);

}  // namespace atk
}  // namespace nn
