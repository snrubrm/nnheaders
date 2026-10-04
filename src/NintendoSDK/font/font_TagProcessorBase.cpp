#include <nn/font/font_TagProcessorBase.h>

#include <nn/font/font_PrintContext.h>
#include <nn/font/font_TextWriterBase.h>

namespace nn::font {

template <typename CharType>
TagProcessorBase<CharType>::TagProcessorBase() {}

template <typename CharType>
TagProcessorBase<CharType>::~TagProcessorBase() {}

template <typename CharType>
void TagProcessorBase<CharType>::BeginPrint(PrintContext<CharType>*) {}

template <typename CharType>
void TagProcessorBase<CharType>::EndPrint(PrintContext<CharType>*) {}

template <typename CharType>
void TagProcessorBase<CharType>::BeginCalculateRect(PrintContext<CharType>*) {}

template <typename CharType>
void TagProcessorBase<CharType>::EndCalculateRect(PrintContext<CharType>*) {}

// 0x7101327b7c (unsigned short), 0x132789c (char)
template <typename CharType>
typename TagProcessorBase<CharType>::Operation TagProcessorBase<CharType>::Process(
    u32 code, PrintContext<CharType>* context) {
    switch (code) {
    case '\n':
        ProcessLinefeed(context);
        return Operation_NextLine;
    case '\t':
        ProcessTab(context);
        return Operation_NoCharSpace;
    default:
        return Operation_Default;
    }
}

template <typename CharType>
void TagProcessorBase<CharType>::ProcessLinefeed(PrintContext<CharType>* context) const {
    TextWriterBase<CharType>& writer = *context->writer;
    const f32 x = context->xOrigin;
    const f32 y = writer.GetCursorY() + writer.GetLineHeight();
    writer.SetCursorX(x);
    writer.SetCursorY(y);
}

template <typename CharType>
void TagProcessorBase<CharType>::ProcessTab(PrintContext<CharType>* context) const {
    TextWriterBase<CharType>& writer = *context->writer;
    const s32 tab_width = writer.GetTabWidth();
    if (tab_width > 0) {
        const f32 char_width = writer.IsWidthFixed() ? writer.GetFixedWidth() : writer.GetFontWidth();
        const f32 dx = writer.GetCursorX() - context->xOrigin;
        const f32 tab_pixels = tab_width * char_width;
        const s32 count = static_cast<s32>(dx / tab_pixels) + 1;
        writer.SetCursorX(context->xOrigin + tab_pixels * count);
    }
}

// NON_MATCHING: same operations; the original loads the writer from the context again after the stores to the
// rectangle and keeps the rectangle edges as pair stores (register / load scheduling only)
// 0x7101327c44 (unsigned short), 0x1327964 (char)
template <typename CharType>
typename TagProcessorBase<CharType>::Operation TagProcessorBase<CharType>::CalculateRect(
    Rectangle* rect, PrintContext<CharType>* context, u32 code) {
    switch (code) {
    case '\n': {
        TextWriterBase<CharType>& writer = *context->writer;
        rect->right = writer.GetCursorX();
        rect->top = writer.GetCursorY();
        ProcessLinefeed(context);
        rect->left = writer.GetCursorX();
        rect->bottom = writer.GetCursorY() + writer.GetFontHeight();
        rect->Normalize();
        return Operation_NextLine;
    }
    case '\t': {
        TextWriterBase<CharType>& writer = *context->writer;
        rect->left = writer.GetCursorX();
        ProcessTab(context);
        rect->right = writer.GetCursorX();
        rect->top = writer.GetCursorY();
        rect->bottom = writer.GetCursorY() + writer.GetFontHeight();
        rect->Normalize();
        return Operation_NoCharSpace;
    }
    default:
        return Operation_Default;
    }
}

template class TagProcessorBase<char>;
template class TagProcessorBase<u16>;

}  // namespace nn::font
