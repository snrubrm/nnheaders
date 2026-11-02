#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

// GetWaveArchiveOfBank 0x71013548f4 writes the id and file address; the sequence
// runtime constructs four consecutive records at 0x7101349dd0.
struct LoadItemInfo {
    u32 mItemId;
    const void* mFileAddress;
};
static_assert(sizeof(LoadItemInfo) == 0x10);

}  // namespace nn::atk::detail
