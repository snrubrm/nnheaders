#include <nn/font/font_Font.h>
#include <nn/util.h>

namespace nn::font {

// 0x7100ab3568
Font::Font() : mKerningEnabled(true), _9(true) {}

// 0x7100ab3584
Font::~Font() {}

// 0x7100ab3680
void Font::Finalize(gfx::Device*) {}

// 0x7100ab4144
// NON_MATCHING: the character buffer and converted code occupy different stack slots.
u32 CharStrmReader::ReadNextCharUtf8() {
    char character[4];
    util::PickOutCharacterFromUtf8String(character, &mStream);
    u32 code = 0;
    util::ConvertCharacterUtf8ToUtf32(&code, character);
    return code;
}

// 0x7100ab4180
u32 CharStrmReader::ReadNextCharUtf16() {
    const u16* stream = reinterpret_cast<const u16*>(mStream);
    const u32 character = *stream++;
    mStream = reinterpret_cast<const char*>(stream);
    return character;
}

// 0x7100ab4194
u32 CharStrmReader::ReadNextCharCp1252() {
    const u8* stream = reinterpret_cast<const u8*>(mStream);
    const u32 character = *stream++;
    mStream = reinterpret_cast<const char*>(stream);
    return character;
}

// 0x7100ab41a8
u32 CharStrmReader::ReadNextCharSjis() {
    const u8* stream = reinterpret_cast<const u8*>(mStream);
    u32 character = stream[0];
    if ((character >= 0x81 && character <= 0x9f) || character >= 0xe0) {
        character = (character << 8) | stream[1];
        stream += 2;
    } else {
        stream += 1;
    }
    mStream = reinterpret_cast<const char*>(stream);
    return character;
}

// NON_MATCHING: same selection; ours keeps a redundant select for the (always zero) adjustment word of the member
// function pointer
// 0x7100ab358c
CharStrmReader Font::GetCharStrmReader(char) const {
    const u32 code = GetCharacterCode();
    CharStrmReader::ReadFunction function =
        code == CharacterCode_Cp1252   ? &CharStrmReader::ReadNextCharCp1252 :
        code == CharacterCode_ShiftJis ? &CharStrmReader::ReadNextCharSjis :
        code == CharacterCode_Unicode  ? &CharStrmReader::ReadNextCharUtf8 :
                                         nullptr;
    return CharStrmReader(nullptr, function);
}

// 0x7100ab35f0
CharStrmReader Font::GetCharStrmReader(u16) const {
    const u32 code = GetCharacterCode();
    return CharStrmReader(nullptr, code == CharacterCode_Unicode ? &CharStrmReader::ReadNextCharUtf16 : nullptr);
}

}  // namespace nn::font
