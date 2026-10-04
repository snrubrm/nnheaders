#pragma once

#include <nn/types.h>

namespace nn::ui2d {

class Layout;
class Pane;

// Interface order is established by eui's derived searcher (vtable 0x24c8d10):
// two destructor entries, UTF16 search, UTF8 search. InitializeString (0xabaae8)
// passes the result, text ID, parent layout, pane, and root layout in that order.
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

    virtual ~TextSearcher() = default;

    // Descriptive names: the original callbacks have no recovered names.
    virtual void SearchText(TextInfo*, const char* text_id, const Layout* parent_layout,
                            const Pane*, const Layout* root_layout) = 0;
    virtual void SearchTextUtf8(TextInfoUtf8*, const char* text_id, const Layout* parent_layout,
                                const Pane*, const Layout* root_layout) = 0;
};
static_assert(sizeof(TextSearcher) == 0x8);
static_assert(sizeof(TextSearcher::TextInfo) == 0x18);
static_assert(sizeof(TextSearcher::TextInfoUtf8) == 0x18);

}  // namespace nn::ui2d
