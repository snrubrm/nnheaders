#include <nn/atk/detail/fnd/FileStreamImpl.h>

namespace nn::atk::detail::fnd {

bool FileStreamImpl::IsOpened() const {
    return mOpened;
}

u32 FileStreamImpl::GetIoBufferAlignment() const {
    return 1;
}

bool FileStreamImpl::CanSetFsAccessLog() const {
    return true;
}

}  // namespace nn::atk::detail::fnd
