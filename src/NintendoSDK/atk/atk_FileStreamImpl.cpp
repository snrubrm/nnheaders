#include <nn/atk/detail/fnd/FileStreamImpl.h>

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

u64 FileStreamImpl::DirectStream::GetSize() const {
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

void FileStreamImpl::SetFsAccessLog(FsAccessLog* log) {
    mFsAccessLog = log;
}

}  // namespace nn::atk::detail::fnd

