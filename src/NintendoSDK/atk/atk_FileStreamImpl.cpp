#include <nn/atk/detail/fnd/FileStreamImpl.h>

#include <nn/diag.h>
#include <nn/fs/fs_files.h>

namespace nn::atk::detail::fnd {

// 0x710135c864
FileStreamImpl::~FileStreamImpl() {}

void FileStreamImpl::DirectStream::Close() {
}

bool FileStreamImpl::IsOpened() const {
    return mOpened;
}

u32 FileStreamImpl::GetIoBufferAlignment() const {
    return 1;
}

bool FileStreamImpl::CanSetFsAccessLog() const {
    return true;
}

// 0x710135c400 / 0x710135c40c / 0x710135c418
bool FileStreamImpl::CanRead() const {
    return IsOpened();
}

bool FileStreamImpl::CanWrite() const {
    return IsOpened();
}

bool FileStreamImpl::CanSeek() const {
    return IsOpened();
}

// 0x710135c4b4
void FileStreamImpl::DisableCache() {
    mStreamCache.Finalize();
}

// 0x710135c4bc
u32 FileStreamImpl::ValidateAlignment(const void*) const {
    return GetIoBufferAlignment();
}

bool FileStreamImpl::DirectStream::IsOpened() const {
    return mStream->IsOpened();
}

u32 FileStreamImpl::DirectStream::GetSize() const {
    return mStream->GetSize();
}

bool FileStreamImpl::DirectStream::CanRead() const {
    return mStream->CanRead();
}

bool FileStreamImpl::DirectStream::CanWrite() const {
    return mStream->CanWrite();
}

bool FileStreamImpl::DirectStream::CanSeek() const {
    return mStream->CanSeek();
}

// 0x710135cae4
u64 FileStreamImpl::DirectStream::GetCurrentPosition() const {
    return mStream->mPosition;
}

void FileStreamImpl::SetFsAccessLog(FsAccessLog* log) {
    mFsAccessLog = log;
}

// 0x710135c5b0
void FileStreamImpl::Close() {
    if (IsOpened()) {
        mPosition = 0;
        mOpened = false;
        nn::fs::CloseFile(mFileHandle);
    }
}

// 0x710135c5f4
void FileStreamImpl::Flush() {
    if (IsOpened())
        nn::fs::FlushFile(mFileHandle);
}

// 0x710135c8a8
bool FileStreamImpl::IsCacheEnabled() const {
    return mStreamCache.isEnabled();
}

// 0x710135c86c
u64 FileStreamImpl::GetCurrentPosition() const {
    if (IsCacheEnabled())
        return mStreamCache.getPosition();
    return mPosition;
}

// 0x710135c638
u32 FileStreamImpl::GetSize() const {
    s64 size;
    if (nn::fs::GetFileSize(&size, mFileHandle).IsSuccess())
        return size;
    nn::diag::detail::AbortImpl("", "", "", 0);
}

// 0x710135c32c
s32 FileStreamImpl::Write(const void* data, u64 size, FndResult* result) {
    if (IsCacheEnabled())
        return mStreamCache.Write(data, size, result);
    return WriteDirect(data, size, result);
}

}  // namespace nn::atk::detail::fnd
