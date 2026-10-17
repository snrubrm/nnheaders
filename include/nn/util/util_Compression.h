#pragma once

#include <nn/types.h>

namespace nn::util {

// Save-transfer decompression 0x710091a694 calls the imported gzip size/query APIs
// (PLT 0x7101800450 / 0x7101800460), with caller-provided output and work buffers.
size_t GetGzipDecompressedSize(const void* src, size_t src_size);
bool DecompressGzip(void* dst, size_t dst_size, const void* src, size_t src_size, void* work,
                    size_t work_size);

}  // namespace nn::util
