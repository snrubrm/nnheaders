#pragma once

#include <nn/types.h>

namespace nn::ui2d {

// Only the nested result records are modelled here; the searcher's interface and layout
// still need to be recovered. TextBox initialization reads these records directly.
class TextSearcher {
public:
    struct TextInfo {
        const u16* mText = nullptr;
        u16 mTextLength = 0;
        u16 _a = 0;
        s32 mBufferLength = -1;
    };
    struct TextInfoUtf8 {
        const char* mText = nullptr;
        u16 mTextLength = 0;
        u16 _a = 0;
        s32 mBufferLength = -1;
    };
};
static_assert(sizeof(TextSearcher::TextInfo) == 0x10);
static_assert(sizeof(TextSearcher::TextInfoUtf8) == 0x10);

}  // namespace nn::ui2d
