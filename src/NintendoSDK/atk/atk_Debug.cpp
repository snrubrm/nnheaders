#include <nn/atk/detail/Debug.h>

namespace nn::atk::detail {

const u32 sUnk_7101ECADC0[] = {31, 1, 2, 4, 8, 16};
u32 sUnk_710252D148 = 31;

// NON_MATCHING: the mutable flag word is accessed through the compiler's GOT entry.
bool Debug_GetWarningFlag(DebugWarningFlag flag) {
    u32 mask = 0;
    if (flag >= 0 && flag < 6)
        mask = sUnk_7101ECADC0[flag];
    return (mask & ~sUnk_710252D148) == 0;
}

// NON_MATCHING: the compiler selects the switch cases without the native branch layout.
DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType(DebugSoundType type) {
    switch (type) {
    case DebugSoundType_Stream:
        return DebugWarningFlag_Stream;
    case DebugSoundType_Wave:
        return DebugWarningFlag_Wave;
    default:
        return DebugWarningFlag_Sequence;
    }
}

}  // namespace nn::atk::detail
