#include <nn/atk/detail/fnd/StreamCache.h>

namespace nn::atk::detail::fnd {

StreamCache::StreamCache()
    : mStream(nullptr), mPosition(0), mBuffer(nullptr), mSize(0), mCachePosition(0xffffffff),
      mCacheSize(0), mCacheState(0) {
}

void StreamCache::Initialize(Stream* stream, void* buffer, u64 size) {
    mStream = stream;
    mPosition = 0;
    mBuffer = buffer;
    mSize = size;
    mCacheState = 0;
    mCachePosition = 0xffffffff;
    mCacheSize = 0;
}

// NON_MATCHING: the compiler moves the cache-state store ahead of the pointer resets.
void StreamCache::Finalize() {
    mBuffer = nullptr;
    mSize = 0;
    mStream = nullptr;
    mPosition = 0;
    mCacheState = 0;
    mCachePosition = 0xffffffff;
    mCacheSize = 0;
}

}  // namespace nn::atk::detail::fnd
