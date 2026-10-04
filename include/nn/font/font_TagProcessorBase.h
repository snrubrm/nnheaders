/**
 * @file font_TagProcessorBase.h
 * @brief Base class of the processors that handle control codes and tags in printed text.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/types.h>

namespace nn::font {

class Rectangle;
template <typename CharType>
class PrintContext;

// Instantiated in the binary for CharType = char and unsigned short (the vtable of eui::TagProcessor extends the
// unsigned short one). Virtual slots (vtable of TagProcessorBase<unsigned short>, 0x252c870): GetRuntimeTypeInfo,
// destructor (D1 / D0), Process, CalculateRect, BeginPrint, EndPrint, BeginCalculateRect, EndCalculateRect.
template <typename CharType>
class TagProcessorBase {
public:
    // Result of Process() / CalculateRect(): what the writer does next. Process returns 3 after a line feed and
    // 1 after a tab, 0 for any other code (TagProcessorBase<unsigned short>::Process 0x7101327b7c).
    enum Operation {
        Operation_Default,
        Operation_NoCharSpace,
        Operation_CharSpace,
        Operation_NextLine,
        Operation_EndDraw,
    };

    NN_RUNTIME_TYPEINFO_BASE();

    TagProcessorBase();
    virtual ~TagProcessorBase();

    // Handles '\n' (line feed) and '\t' (tab); every other code is left to the writer.
    virtual Operation Process(u32 code, PrintContext<CharType>* context);
    virtual Operation CalculateRect(Rectangle* rect, PrintContext<CharType>* context, u32 code);
    virtual void BeginPrint(PrintContext<CharType>* context);
    virtual void EndPrint(PrintContext<CharType>* context);
    virtual void BeginCalculateRect(PrintContext<CharType>* context);
    virtual void EndCalculateRect(PrintContext<CharType>* context);
};

}  // namespace nn::font
