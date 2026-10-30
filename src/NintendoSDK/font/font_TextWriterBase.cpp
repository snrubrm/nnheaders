#include <nn/font/font_TextWriterBase.h>
#include <nn/font/font_TagProcessorBase.h>
#include <nn/font/font_PrintContext.h>
#include <nn/util/util_BytePtr.h>

#include <limits>
#include <algorithm>
#include <cmath>

namespace nn::font {

template <typename CharType>
TagProcessorBase<CharType> TextWriterBase<CharType>::sDefaultTagProcessor;

template <typename CharType>
TextWriterBase<CharType>::TextWriterBase()
    : mWidthLimit(std::numeric_limits<f32>::max()), mCharSpace(0.0f), mLineSpace(0.0f), _48(0.0f),
      mTabWidth(4), mDrawFlag(0), mTagProcessor(&sDefaultTagProcessor), _60(0), _61(0) {}

// 0x7101329100
template <typename CharType>
TextWriterBase<CharType>::~TextWriterBase() {}

// 0x7101329104
template <typename CharType>
f32 TextWriterBase<CharType>::GetLineHeight() const {
    f32 height = 0.0f;
    if (mFont)
        height = mFont->GetLineFeed();
    return height * mScaleY + mLineSpace;
}

// 0x7101328578 (char) / 0x71013298f0 (u16)
// NON_MATCHING: the compiler loads and stores cursor scalars separately.
template <typename CharType>
f32 TextWriterBase<CharType>::Print(const CharType* string, s32 length) {
    TextWriterBase writer(*this);
    const f32 width = writer.PrintImpl(string, length, 0, nullptr, nullptr);
    mCursorX = writer.mCursorX;
    mCursorY = writer.mCursorY;
    return width;
}

// 0x71013285f8 (char) / 0x7101329970 (u16)
// NON_MATCHING: the compiler loads and stores cursor scalars separately.
template <typename CharType>
f32 TextWriterBase<CharType>::Print(const CharType* string, s32 length, s32 arg3,
                                  const f32* arg4, const f32* arg5) {
    TextWriterBase writer(*this);
    const f32 width = writer.PrintImpl(string, length, arg3, arg4, arg5);
    mCursorX = writer.mCursorX;
    mCursorY = writer.mCursorY;
    return width;
}

// NON_MATCHING: shared line-alignment measurement and control-flow scheduling differ.
// 0x710132868c (char) / 0x7101329a04 (u16)
template <typename CharType>
f32 TextWriterBase<CharType>::PrintImpl(const CharType* string, s32 length, s32 arg3,
                                       const f32* arg4, const f32* arg5) {
    f32 origin_x = mCursorX;
    const f32 original_y = mCursorY;
    f32 origin_y = original_y;
    const f32 width_limit = mWidthLimit;
    f32 bounds_width = AdjustCursor(&origin_x, &origin_y, string, length);
    const f32 adjusted_y = mCursorY;
    PrintContext<CharType> context{this, string, string + length, origin_x, origin_y,
                                  mScaleX, mScaleY, 0, 0};
    if (arg3 != 0)
        mCursorX = arg3 > 0 ? arg4[0] : 0.0f;
    CharStrmReader reader = mFont->GetCharStrmReader(CharType{});
    reader.mStream = static_cast<const char*>(static_cast<const void*>(string));
    mTagProcessor->BeginPrint(&context);

    const bool width_limited = width_limit < std::numeric_limits<f32>::max();
    const char* line_start = static_cast<const char*>(static_cast<const void*>(string));
    const char* wrap_position = line_start;
    s32 line_index = 0;
    bool char_space = false;
    f32 advance = 0.0f;
    u32 code = reader.Next();
    while (util::ConstBytePtr(reader.mStream).Get<CharType>() - string <= length) {
        const bool can_wrap = width_limited &&
                              (line_index < arg3 || wrap_position != line_start);
        if (code < 0x20) {
            context.flags = char_space ? 0 : 1;
            context.str = util::ConstBytePtr(reader.mStream).Get<CharType>();
            bool wrap_tag = false;
            if (can_wrap && code != '\n') {
                PrintContext<CharType> preview_context(context);
                TextWriterBase preview_writer(*this);
                preview_context.writer = &preview_writer;
                Rectangle preview_rect{};
                mTagProcessor->CalculateRect(&preview_rect, &preview_context, code);
                if (preview_rect.right - preview_rect.left > 0.0f) {
                    const f32 allowance = line_index < arg3 ? arg5[line_index] : 0.0f;
                    wrap_tag = preview_writer.mCursorX - context.xOrigin > mWidthLimit + allowance;
                }
            }
            if (wrap_tag) {
                code = '\n';
                context.previousChar = 0;
                reader.mStream = wrap_position;
                continue;
            }
            const auto operation = mTagProcessor->Process(code, &context);
            const f32 tag_x = mCursorX;
            if (code == '\n' && _61 == 0)
                context.previousChar = 0;
            if (operation == TagProcessorBase<CharType>::Operation_EndDraw)
                break;
            switch (operation) {
            case TagProcessorBase<CharType>::Operation_NoCharSpace:
                char_space = false;
                break;
            case TagProcessorBase<CharType>::Operation_CharSpace:
                char_space = true;
                break;
            case TagProcessorBase<CharType>::Operation_NextLine: {
                ++line_index;
                const f32 line_offset = line_index < arg3 ? arg4[line_index] : 0.0f;
                const u32 alignment = mDrawFlag & 3;
                f32 position;
                if (alignment == 1 || alignment == 2) {
                    f32 line_width;
                    {
                        const CharType* next = context.str;
                        Rectangle line{};
                        TextWriterBase writer(*this);
                        writer.SetCursorX(0.0f);
                        writer.SetCursorY(0.0f);
                        writer.CalculateLineRectImpl(&line, &next,
                            length - s32(context.str - string));
                        line_width = line.right - line.left;
                    }
                    if (alignment == 1) {
                        f32 bounds_half = bounds_width * 0.5f;
                        f32 line_half = line_width * 0.5f;
                        if (_60) {
                            bounds_half = std::ceil(bounds_half);
                            line_half = std::ceil(line_half);
                        }
                        position = context.xOrigin + (bounds_half - line_half);
                    } else {
                        position = context.xOrigin + (bounds_width - line_width);
                    }
                } else {
                    bounds_width = std::max(bounds_width, tag_x - context.xOrigin);
                    position = context.xOrigin;
                }
                mCursorX = line_offset + position;
                char_space = false;
                if (width_limited)
                    line_start = reader.mStream;
                break;
            }
            default:
                break;
            }
            advance = tag_x - origin_x;
            reader.mStream = static_cast<const char*>(static_cast<const void*>(context.str));
        } else {
            const f32 saved_y = mCursorY;
            const s32 kerning_value = mFont->GetKerning(context.previousChar, code);
            const f32 kerning = kerning_value * mScaleX;
            Glyph glyph;
            mFont->GetGlyph(&glyph, code);
            if (glyph.height > glyph.texHeight) {
                code = reader.Next();
                continue;
            }
            f32 position = advance;
            if (char_space)
                position += mCharSpace;
            position += mIsWidthFixed ? mFixedWidth : glyph.charWidth * mScaleX;
            const s32 trailing_value = mFont->GetKerning(code, 0);
            const f32 trailing_kerning = trailing_value * mScaleX;
            const f32 right = (kerning + position) + trailing_kerning;
            if (can_wrap) {
                const f32 allowance = line_index < arg3 ? arg5[line_index] : 0.0f;
                if (right > mWidthLimit + allowance) {
                    context.previousChar = 0;
                    code = '\n';
                    reader.mStream = wrap_position;
                    continue;
                }
            }
            if (char_space)
                mCursorX += mCharSpace;
            advance = right - trailing_kerning;
            mCursorX += kerning;
            const s32 baseline = mFont->GetBaselinePos();
            mCursorY += -(baseline + glyph._28) * mScaleY;
            PrintGlyph(glyph);
            mCursorY = saved_y;
            char_space = true;
            context.previousChar = code;
        }
        if (width_limited)
            wrap_position = reader.mStream;
        code = reader.Next();
    }
    const s32 end_kerning = mFont->GetKerning(context.previousChar, 0);
    mCursorX += end_kerning * mScaleX;
    bounds_width = std::max(bounds_width, mCursorX - context.xOrigin);
    const u32 vertical = mDrawFlag & 0x300;
    if (vertical == 0x100 || vertical == 0x200)
        mCursorY = original_y;
    else
        mCursorY = (original_y - adjusted_y) + mCursorY;
    mTagProcessor->EndPrint(&context);
    return bounds_width;
}

// 0x7101328db4 (char) / 0x710132a13c (u16)
// NON_MATCHING: natural shared alignment logic changes branch structure and local writer scheduling.
template <typename CharType>
f32 TextWriterBase<CharType>::AdjustCursor(f32* x, f32* y, const CharType* string, s32 length) {
    f32 bounds_x = 0.0f;
    f32 bounds_y = 0.0f;
    if ((mDrawFlag & 0x333) != 0 && (mDrawFlag & 0x333) != 0x300) {
        Rectangle rect{};
        CalculateStringRect(&rect, string, length);
        bounds_x = rect.left + rect.right;
        bounds_y = rect.top + rect.bottom;
    }

    switch (mDrawFlag & 0x30) {
    case 0x10: {
        f32 offset = bounds_x * 0.5f;
        if (static_cast<u8>(_60))
            offset = std::ceil(offset);
        *x -= offset;
        break;
    }
    case 0x20:
        *x -= bounds_x;
        break;
    }
    switch (mDrawFlag & 0x300) {
    case 0x100: {
        f32 offset = bounds_y * 0.5f;
        if (static_cast<u8>(_60))
            offset = std::ceil(offset);
        *y -= offset;
        break;
    }
    case 0x200:
        *y -= bounds_y;
        break;
    }

    f32 cursor_x = *x;
    const u32 line_alignment = mDrawFlag & 3;
    if (line_alignment == 1 || line_alignment == 2) {
        f32 line_width;
        {
            TextWriterBase writer(*this);
            Rectangle line{};
            const CharType* next = string;
            writer.SetCursorX(0.0f);
            writer.SetCursorY(0.0f);
            writer.CalculateLineRectImpl(&line, &next, length);
            line_width = line.right - line.left;
        }
        if (line_alignment == 1) {
            f32 bounds_half = bounds_x * 0.5f;
            f32 line_half = line_width * 0.5f;
            if (static_cast<u8>(_60)) {
                bounds_half = std::ceil(bounds_half);
                line_half = std::ceil(line_half);
            }
            cursor_x = *x + (bounds_half - line_half);
        } else {
            cursor_x = bounds_x - line_width + *x;
        }
    }
    SetCursorX(cursor_x);
    f32 cursor_y = *y;
    if ((mDrawFlag & 0x300) != 0x300)
        cursor_y += GetFontAscent();
    SetCursorY(cursor_y);
    return bounds_x;
}

// NON_MATCHING: bounds lowering, loop control flow and preview cleanup differ.
// 0x7101327fac (char) / 0x7101329320 (u16)
template <typename CharType>
bool TextWriterBase<CharType>::CalculateLineRectImpl(Rectangle* rect, const CharType** string,
                                                    s32 length) {
    const CharType* begin = *string;
    const CharType* end = begin + length;
    const f32 width_limit = mWidthLimit;
    const bool width_limited = width_limit < std::numeric_limits<f32>::max();
    PrintContext<CharType> context{this, begin, end, 0.0f, 0.0f, mScaleX, mScaleY, 0, 0};
    CharStrmReader reader = mFont->GetCharStrmReader(CharType{});
    reader.mStream = static_cast<const char*>(static_cast<const void*>(begin));
    const f32 line_height = GetLineHeight() - mLineSpace;
    *rect = Rectangle(0.0f, std::min(0.0f, line_height), 0.0f,
                      std::max(0.0f, line_height));
    mTagProcessor->BeginCalculateRect(&context);

    f32 cursor = 0.0f;
    bool char_space = false;
    bool wrapped = false;
    const char* wrap_position = nullptr;
    u32 code = reader.Next();
    while (util::ConstBytePtr(reader.mStream).Get<CharType>() <= end) {
        typename TagProcessorBase<CharType>::Operation operation;
        if (code < 0x20) {
            if (code == '\n' && _61 == 0) {
                cursor += mFont->GetKerning(context.previousChar, 0) * mScaleX;
                context.previousChar = 0;
            }
            Rectangle tag_rect(cursor, 0.0f, 0.0f, 0.0f);
            context.str = util::ConstBytePtr(reader.mStream).Get<CharType>();
            context.flags = char_space ? 0 : 1;
            mCursorX = cursor;
            bool wrap_tag = false;
            if (width_limited && wrap_position && code != '\n') {
                PrintContext<CharType> preview_context(context);
                TextWriterBase preview_writer(*this);
                preview_context.writer = &preview_writer;
                Rectangle preview_rect{};
                mTagProcessor->CalculateRect(&preview_rect, &preview_context, code);
                wrap_tag = preview_rect.right - preview_rect.left > 0.0f &&
                           preview_writer.mCursorX - context.xOrigin > mWidthLimit;
            }
            if (wrap_tag) {
                code = '\n';
                wrapped = true;
                reader.mStream = wrap_position;
                continue;
            }
            operation = mTagProcessor->CalculateRect(&tag_rect, &context, code);
            reader.mStream = static_cast<const char*>(static_cast<const void*>(context.str));
            rect->left = std::min(rect->left, tag_rect.left);
            rect->top = std::min(rect->top, tag_rect.top);
            rect->right = std::max(rect->right, tag_rect.right);
            rect->bottom = std::max(rect->bottom, tag_rect.bottom);
            cursor = mCursorX;
        } else {
            f32 position = cursor;
            if (char_space)
                position += mCharSpace;
            f32 char_width;
            if (mIsWidthFixed)
                char_width = mFixedWidth;
            else
                char_width = mFont->GetCharWidth(code) * mScaleX;
            position += char_width;
            position += mFont->GetKerning(context.previousChar, code) * mScaleX;
            const f32 trailing_kerning = mFont->GetKerning(code, 0) * mScaleX;
            const f32 right = position + trailing_kerning;
            if (width_limited && wrap_position && right + _48 > mWidthLimit) {
                wrapped = true;
                code = '\n';
                reader.mStream = wrap_position;
                continue;
            }
            rect->left = std::min(rect->left, right);
            rect->right = std::max(rect->right, right);
            cursor = right - trailing_kerning;
            context.previousChar = code;
            operation = TagProcessorBase<CharType>::Operation_CharSpace;
        }
        if (operation == TagProcessorBase<CharType>::Operation_NextLine)
            break;
        if (operation == TagProcessorBase<CharType>::Operation_EndDraw) {
            *string += length;
            return false;
        }
        switch (operation) {
        case TagProcessorBase<CharType>::Operation_NoCharSpace:
            char_space = false;
            break;
        case TagProcessorBase<CharType>::Operation_CharSpace:
            char_space = true;
            break;
        default:
            break;
        }
        if (width_limited)
            wrap_position = reader.mStream;
        code = reader.Next();
    }
    const f32 right = cursor + mFont->GetKerning(context.previousChar, 0) * mScaleX;
    rect->left = std::min(rect->left, right);
    rect->right = std::max(rect->right, right);
    *string = util::ConstBytePtr(reader.mStream).Get<CharType>();
    mTagProcessor->EndCalculateRect(&context);
    return wrapped;
}

// 0x7101329190
// NON_MATCHING: null-string branch and line-bound update scheduling.
template <>
void TextWriterBase<u16>::CalculateStringRect(Rectangle* rect, const u16* string, s32 length) const {
    if (!string) {
        *rect = {};
        return;
    }

    TextWriterBase writer(*this);
    const u16* end = string + length;
    *rect = {};
    writer.SetCursorX(0.0f);
    writer.SetCursorY(0.0f);
    do {
        Rectangle line{};
        writer.CalculateLineRectImpl(&line, &string, length);
        rect->left = std::min(rect->left, line.left);
        rect->top = std::min(rect->top, line.top);
        rect->right = std::max(rect->right, line.right);
        rect->bottom = std::max(rect->bottom, line.bottom);
        length = end - string;
    } while (length > 0);
}

// 0x7101327e88
// NON_MATCHING: null-string branch and line-bound update scheduling.
template <>
void TextWriterBase<char>::CalculateStringRect(Rectangle* rect, const char* string, s32 length) const {
    if (!string) {
        *rect = {};
        return;
    }

    TextWriterBase writer(*this);
    const char* end = string + length;
    *rect = {};
    writer.SetCursorX(0.0f);
    writer.SetCursorY(0.0f);
    do {
        Rectangle line{};
        writer.CalculateLineRectImpl(&line, &string, length);
        rect->left = std::min(rect->left, line.left);
        rect->top = std::min(rect->top, line.top);
        rect->right = std::max(rect->right, line.right);
        rect->bottom = std::max(rect->bottom, line.bottom);
        length = end - string;
    } while (length > 0);
}

// 0x7101329150
template <>
f32 TextWriterBase<u16>::CalculateStringWidth(const u16* string, s32 length) const {
    Rectangle rect{};
    CalculateStringRect(&rect, string, length);
    return rect.right - rect.left;
}

// 0x71013292b8
template <>
const u16* TextWriterBase<u16>::FindPosOfWidthLimit(const u16* string, s32 length) const {
    Rectangle rect{};
    TextWriterBase writer(*this);
    writer.SetCursorX(0.0f);
    writer.SetCursorY(0.0f);
    writer.CalculateLineRectImpl(&rect, &string, length);
    return string;
}

template class TextWriterBase<char>;
template class TextWriterBase<u16>;

}  // namespace nn::font
