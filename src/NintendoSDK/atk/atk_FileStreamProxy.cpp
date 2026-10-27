#include <nn/atk/detail/fnd/FileStreamProxy.h>

namespace nn::atk::detail::fnd {

void FileStreamProxy::Close() {
    mStream->Close();
}

bool FileStreamProxy::IsOpened() const {
    return mStream->IsOpened();
}

s32 FileStreamProxy::Read(void* data, u64 size, FndResult* result) {
    return mStream->Read(data, size, result);
}

s32 FileStreamProxy::Write(const void* data, u64 size, FndResult* result) {
    return mStream->Write(data, size, result);
}

u64 FileStreamProxy::GetCurrentPosition() const {
    return mStream->GetCurrentPosition();
}

u64 FileStreamProxy::GetSize() const {
    return mStream->GetSize();
}

bool FileStreamProxy::CanRead() const {
    return mStream->CanRead();
}

bool FileStreamProxy::CanWrite() const {
    return mStream->CanWrite();
}

bool FileStreamProxy::CanSeek() const {
    return mStream->CanSeek();
}

void FileStreamProxy::Flush() {
    mStream->Flush();
}

void FileStreamProxy::EnableCache(void* buffer, u64 size) {
    mStream->EnableCache(buffer, size);
}

void FileStreamProxy::DisableCache() {
    mStream->DisableCache();
}

bool FileStreamProxy::IsCacheEnabled() const {
    return mStream->IsCacheEnabled();
}

u32 FileStreamProxy::GetIoBufferAlignment() const {
    return mStream->GetIoBufferAlignment();
}

bool FileStreamProxy::CanSetFsAccessLog() const {
    return mStream->CanSetFsAccessLog();
}

void FileStreamProxy::SetFsAccessLog(FsAccessLog* log) {
    mStream->SetFsAccessLog(log);
}

}  // namespace nn::atk::detail::fnd
