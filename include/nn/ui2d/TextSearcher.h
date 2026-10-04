#pragma once

#include <nn/types.h>

namespace nn::ui2d {

// Only the nested result records are modelled here; the searcher's interface and layout
// still need to be recovered. TextBox initialization reads these records directly.
class TextSearcher {
public:
    struct TextInfo {
        const u16* mText = nullptr;
        u32 mTextLength = 0;
        u32 mBufferLength = 0;
        s32 _10 = -1;
    };
    struct TextInfoUtf8 {
        const char* mText = nullptr;
        u32 mTextLength = 0;
        u32 mBufferLength = 0;
        s32 _10 = -1;
    };
};
static_assert(sizeof(TextSearcher::TextInfo) == 0x18);
static_assert(sizeof(TextSearcher::TextInfoUtf8) == 0x18);

}  // namespace nn::ui2d
