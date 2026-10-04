#include <nn/font/font_TagProcessorBase.h>

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

// Process() and CalculateRect() (0x7101327b7c / 0x7101327c44 and the char twins) need nn::font::TextWriterBase and
// CharWriter and are not defined yet.

template class TagProcessorBase<char>;
template class TagProcessorBase<u16>;

}  // namespace nn::font
