#pragma once

#include <nn/types.h>

namespace nn::font {

// Partial call interface only. Slot 0's parameters and pointer result are established
// by FindLineBreak and its existing eui provider; invoke is an inferred name.
// Construction, default implementation, purity and the remaining class are unclaimed.
class WordWrapCallback {
public:
    virtual const u16* invoke(const u16* begin, const u16* end);
};

// Observed prefix only. Defaults, byte meanings and the remaining record are unclaimed.
struct WordWrapConfig {
    s32 _0;
    u8 _4;
};

// Non-instance wrapping API; no object layout or lifetime is represented.
namespace WordWrapping {
const u16* FindLineBreak(const u16*, const u16*, WordWrapCallback&, const WordWrapConfig&);
// Return type inferred from the native normalized 0/1 result.
bool CalculateWordWrapping(u32*, u16*, u32, const u16*, u32, WordWrapCallback&,
                           const WordWrapConfig&);
}

}  // namespace nn::font
