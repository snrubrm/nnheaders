#pragma once

#include <nn/types.h>

namespace nn::atk {

// Partial names inferred from the sequence, stream and wave warning callers.
enum DebugWarningFlag : s32 {
    DebugWarningFlag_Sequence = 1,
    DebugWarningFlag_Stream = 2,
    DebugWarningFlag_Wave = 3,
};

namespace detail {

enum DebugSoundType : s32 {
    DebugSoundType_Sequence = 0,
    DebugSoundType_Stream = 1,
    DebugSoundType_Wave = 2,
};

bool Debug_GetWarningFlag(DebugWarningFlag flag);
DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType(DebugSoundType type);

}  // namespace detail
}  // namespace nn::atk
