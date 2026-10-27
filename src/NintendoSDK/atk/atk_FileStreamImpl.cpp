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

