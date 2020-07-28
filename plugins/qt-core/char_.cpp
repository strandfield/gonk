// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "char_.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/binding/enum.h"
#include "gonk/common/binding/namespace.h"
#include "gonk/common/enums.h"

#include <script/classbuilder.h>
#include <script/enumbuilder.h>

#include "qt-core/char_.h"
#include "qt-core/string.h"

#if METAGONK_SOURCE
{% assign current_class = project | get_symbol: 'Qt.Core', 'QLatin1Char' %}
{% include generate_class with class = current_class and recursive = true %}
{% assign current_class = project | get_symbol: 'Qt.Core', 'QChar' %}
{% include generate_class with class = current_class and recursive = true %}
#else

static void register_latin1_char(script::Namespace& parent)
{
  using namespace script;
  
  Class latin1_char = parent.newClass("QLatin1Char").setId(script::Type::make<QLatin1Char>().data())
    .get();

  Class& c = latin1_char;
  

  // QLatin1Char(char c);
  gonk::bind::constructor<QLatin1Char, char>(c).create();
  // char toLatin1() const;
  gonk::bind::member_function<QLatin1Char, char, &QLatin1Char::toLatin1>(c, "toLatin1").create();
  // ushort unicode() const;
  /// TODO: ushort unicode() const;
}
static void register_char__special_character(script::Class& parent)
{
  using namespace script;
  
  Enum special_character = gonk::bind::enumeration<QChar::SpecialCharacter>(parent, "SpecialCharacter").get();

  special_character.addValue("Null", QChar::SpecialCharacter::Null);
  special_character.addValue("Tabulation", QChar::SpecialCharacter::Tabulation);
  special_character.addValue("LineFeed", QChar::SpecialCharacter::LineFeed);
  special_character.addValue("FormFeed", QChar::SpecialCharacter::FormFeed);
  special_character.addValue("CarriageReturn", QChar::SpecialCharacter::CarriageReturn);
  special_character.addValue("Space", QChar::SpecialCharacter::Space);
  special_character.addValue("Nbsp", QChar::SpecialCharacter::Nbsp);
  special_character.addValue("SoftHyphen", QChar::SpecialCharacter::SoftHyphen);
  special_character.addValue("ReplacementCharacter", QChar::SpecialCharacter::ReplacementCharacter);
  special_character.addValue("ObjectReplacementCharacter", QChar::SpecialCharacter::ObjectReplacementCharacter);
  special_character.addValue("ByteOrderMark", QChar::SpecialCharacter::ByteOrderMark);
  special_character.addValue("ByteOrderSwapped", QChar::SpecialCharacter::ByteOrderSwapped);
  special_character.addValue("ParagraphSeparator", QChar::SpecialCharacter::ParagraphSeparator);
  special_character.addValue("LineSeparator", QChar::SpecialCharacter::LineSeparator);
  special_character.addValue("LastValidCodePoint", QChar::SpecialCharacter::LastValidCodePoint);
}
static void register_char__category(script::Class& parent)
{
  using namespace script;
  
  Enum category = gonk::bind::enumeration<QChar::Category>(parent, "Category").get();

  category.addValue("Mark_NonSpacing", QChar::Category::Mark_NonSpacing);
  category.addValue("Mark_SpacingCombining", QChar::Category::Mark_SpacingCombining);
  category.addValue("Mark_Enclosing", QChar::Category::Mark_Enclosing);
  category.addValue("Number_DecimalDigit", QChar::Category::Number_DecimalDigit);
  category.addValue("Number_Letter", QChar::Category::Number_Letter);
  category.addValue("Number_Other", QChar::Category::Number_Other);
  category.addValue("Separator_Space", QChar::Category::Separator_Space);
  category.addValue("Separator_Line", QChar::Category::Separator_Line);
  category.addValue("Separator_Paragraph", QChar::Category::Separator_Paragraph);
  category.addValue("Other_Control", QChar::Category::Other_Control);
  category.addValue("Other_Format", QChar::Category::Other_Format);
  category.addValue("Other_Surrogate", QChar::Category::Other_Surrogate);
  category.addValue("Other_PrivateUse", QChar::Category::Other_PrivateUse);
  category.addValue("Other_NotAssigned", QChar::Category::Other_NotAssigned);
  category.addValue("Letter_Uppercase", QChar::Category::Letter_Uppercase);
  category.addValue("Letter_Lowercase", QChar::Category::Letter_Lowercase);
  category.addValue("Letter_Titlecase", QChar::Category::Letter_Titlecase);
  category.addValue("Letter_Modifier", QChar::Category::Letter_Modifier);
  category.addValue("Letter_Other", QChar::Category::Letter_Other);
  category.addValue("Punctuation_Connector", QChar::Category::Punctuation_Connector);
  category.addValue("Punctuation_Dash", QChar::Category::Punctuation_Dash);
  category.addValue("Punctuation_Open", QChar::Category::Punctuation_Open);
  category.addValue("Punctuation_Close", QChar::Category::Punctuation_Close);
  category.addValue("Punctuation_InitialQuote", QChar::Category::Punctuation_InitialQuote);
  category.addValue("Punctuation_FinalQuote", QChar::Category::Punctuation_FinalQuote);
  category.addValue("Punctuation_Other", QChar::Category::Punctuation_Other);
  category.addValue("Symbol_Math", QChar::Category::Symbol_Math);
  category.addValue("Symbol_Currency", QChar::Category::Symbol_Currency);
  category.addValue("Symbol_Modifier", QChar::Category::Symbol_Modifier);
  category.addValue("Symbol_Other", QChar::Category::Symbol_Other);
}
static void register_char__script(script::Class& parent)
{
  using namespace script;
  
  Enum script = gonk::bind::enumeration<QChar::Script>(parent, "Script").get();

  script.addValue("Script_Unknown", QChar::Script::Script_Unknown);
  script.addValue("Script_Inherited", QChar::Script::Script_Inherited);
  script.addValue("Script_Common", QChar::Script::Script_Common);
  script.addValue("Script_Latin", QChar::Script::Script_Latin);
  script.addValue("Script_Greek", QChar::Script::Script_Greek);
  script.addValue("Script_Cyrillic", QChar::Script::Script_Cyrillic);
  script.addValue("Script_Armenian", QChar::Script::Script_Armenian);
  script.addValue("Script_Hebrew", QChar::Script::Script_Hebrew);
  script.addValue("Script_Arabic", QChar::Script::Script_Arabic);
  script.addValue("Script_Syriac", QChar::Script::Script_Syriac);
  script.addValue("Script_Thaana", QChar::Script::Script_Thaana);
  script.addValue("Script_Devanagari", QChar::Script::Script_Devanagari);
  script.addValue("Script_Bengali", QChar::Script::Script_Bengali);
  script.addValue("Script_Gurmukhi", QChar::Script::Script_Gurmukhi);
  script.addValue("Script_Gujarati", QChar::Script::Script_Gujarati);
  script.addValue("Script_Oriya", QChar::Script::Script_Oriya);
  script.addValue("Script_Tamil", QChar::Script::Script_Tamil);
  script.addValue("Script_Telugu", QChar::Script::Script_Telugu);
  script.addValue("Script_Kannada", QChar::Script::Script_Kannada);
  script.addValue("Script_Malayalam", QChar::Script::Script_Malayalam);
  script.addValue("Script_Sinhala", QChar::Script::Script_Sinhala);
  script.addValue("Script_Thai", QChar::Script::Script_Thai);
  script.addValue("Script_Lao", QChar::Script::Script_Lao);
  script.addValue("Script_Tibetan", QChar::Script::Script_Tibetan);
  script.addValue("Script_Myanmar", QChar::Script::Script_Myanmar);
  script.addValue("Script_Georgian", QChar::Script::Script_Georgian);
  script.addValue("Script_Hangul", QChar::Script::Script_Hangul);
  script.addValue("Script_Ethiopic", QChar::Script::Script_Ethiopic);
  script.addValue("Script_Cherokee", QChar::Script::Script_Cherokee);
  script.addValue("Script_CanadianAboriginal", QChar::Script::Script_CanadianAboriginal);
  script.addValue("Script_Ogham", QChar::Script::Script_Ogham);
  script.addValue("Script_Runic", QChar::Script::Script_Runic);
  script.addValue("Script_Khmer", QChar::Script::Script_Khmer);
  script.addValue("Script_Mongolian", QChar::Script::Script_Mongolian);
  script.addValue("Script_Hiragana", QChar::Script::Script_Hiragana);
  script.addValue("Script_Katakana", QChar::Script::Script_Katakana);
  script.addValue("Script_Bopomofo", QChar::Script::Script_Bopomofo);
  script.addValue("Script_Han", QChar::Script::Script_Han);
  script.addValue("Script_Yi", QChar::Script::Script_Yi);
  script.addValue("Script_OldItalic", QChar::Script::Script_OldItalic);
  script.addValue("Script_Gothic", QChar::Script::Script_Gothic);
  script.addValue("Script_Deseret", QChar::Script::Script_Deseret);
  script.addValue("Script_Tagalog", QChar::Script::Script_Tagalog);
  script.addValue("Script_Hanunoo", QChar::Script::Script_Hanunoo);
  script.addValue("Script_Buhid", QChar::Script::Script_Buhid);
  script.addValue("Script_Tagbanwa", QChar::Script::Script_Tagbanwa);
  script.addValue("Script_Coptic", QChar::Script::Script_Coptic);
  script.addValue("Script_Limbu", QChar::Script::Script_Limbu);
  script.addValue("Script_TaiLe", QChar::Script::Script_TaiLe);
  script.addValue("Script_LinearB", QChar::Script::Script_LinearB);
  script.addValue("Script_Ugaritic", QChar::Script::Script_Ugaritic);
  script.addValue("Script_Shavian", QChar::Script::Script_Shavian);
  script.addValue("Script_Osmanya", QChar::Script::Script_Osmanya);
  script.addValue("Script_Cypriot", QChar::Script::Script_Cypriot);
  script.addValue("Script_Braille", QChar::Script::Script_Braille);
  script.addValue("Script_Buginese", QChar::Script::Script_Buginese);
  script.addValue("Script_NewTaiLue", QChar::Script::Script_NewTaiLue);
  script.addValue("Script_Glagolitic", QChar::Script::Script_Glagolitic);
  script.addValue("Script_Tifinagh", QChar::Script::Script_Tifinagh);
  script.addValue("Script_SylotiNagri", QChar::Script::Script_SylotiNagri);
  script.addValue("Script_OldPersian", QChar::Script::Script_OldPersian);
  script.addValue("Script_Kharoshthi", QChar::Script::Script_Kharoshthi);
  script.addValue("Script_Balinese", QChar::Script::Script_Balinese);
  script.addValue("Script_Cuneiform", QChar::Script::Script_Cuneiform);
  script.addValue("Script_Phoenician", QChar::Script::Script_Phoenician);
  script.addValue("Script_PhagsPa", QChar::Script::Script_PhagsPa);
  script.addValue("Script_Nko", QChar::Script::Script_Nko);
  script.addValue("Script_Sundanese", QChar::Script::Script_Sundanese);
  script.addValue("Script_Lepcha", QChar::Script::Script_Lepcha);
  script.addValue("Script_OlChiki", QChar::Script::Script_OlChiki);
  script.addValue("Script_Vai", QChar::Script::Script_Vai);
  script.addValue("Script_Saurashtra", QChar::Script::Script_Saurashtra);
  script.addValue("Script_KayahLi", QChar::Script::Script_KayahLi);
  script.addValue("Script_Rejang", QChar::Script::Script_Rejang);
  script.addValue("Script_Lycian", QChar::Script::Script_Lycian);
  script.addValue("Script_Carian", QChar::Script::Script_Carian);
  script.addValue("Script_Lydian", QChar::Script::Script_Lydian);
  script.addValue("Script_Cham", QChar::Script::Script_Cham);
  script.addValue("Script_TaiTham", QChar::Script::Script_TaiTham);
  script.addValue("Script_TaiViet", QChar::Script::Script_TaiViet);
  script.addValue("Script_Avestan", QChar::Script::Script_Avestan);
  script.addValue("Script_EgyptianHieroglyphs", QChar::Script::Script_EgyptianHieroglyphs);
  script.addValue("Script_Samaritan", QChar::Script::Script_Samaritan);
  script.addValue("Script_Lisu", QChar::Script::Script_Lisu);
  script.addValue("Script_Bamum", QChar::Script::Script_Bamum);
  script.addValue("Script_Javanese", QChar::Script::Script_Javanese);
  script.addValue("Script_MeeteiMayek", QChar::Script::Script_MeeteiMayek);
  script.addValue("Script_ImperialAramaic", QChar::Script::Script_ImperialAramaic);
  script.addValue("Script_OldSouthArabian", QChar::Script::Script_OldSouthArabian);
  script.addValue("Script_InscriptionalParthian", QChar::Script::Script_InscriptionalParthian);
  script.addValue("Script_InscriptionalPahlavi", QChar::Script::Script_InscriptionalPahlavi);
  script.addValue("Script_OldTurkic", QChar::Script::Script_OldTurkic);
  script.addValue("Script_Kaithi", QChar::Script::Script_Kaithi);
  script.addValue("Script_Batak", QChar::Script::Script_Batak);
  script.addValue("Script_Brahmi", QChar::Script::Script_Brahmi);
  script.addValue("Script_Mandaic", QChar::Script::Script_Mandaic);
  script.addValue("Script_Chakma", QChar::Script::Script_Chakma);
  script.addValue("Script_MeroiticCursive", QChar::Script::Script_MeroiticCursive);
  script.addValue("Script_MeroiticHieroglyphs", QChar::Script::Script_MeroiticHieroglyphs);
  script.addValue("Script_Miao", QChar::Script::Script_Miao);
  script.addValue("Script_Sharada", QChar::Script::Script_Sharada);
  script.addValue("Script_SoraSompeng", QChar::Script::Script_SoraSompeng);
  script.addValue("Script_Takri", QChar::Script::Script_Takri);
  script.addValue("Script_CaucasianAlbanian", QChar::Script::Script_CaucasianAlbanian);
  script.addValue("Script_BassaVah", QChar::Script::Script_BassaVah);
  script.addValue("Script_Duployan", QChar::Script::Script_Duployan);
  script.addValue("Script_Elbasan", QChar::Script::Script_Elbasan);
  script.addValue("Script_Grantha", QChar::Script::Script_Grantha);
  script.addValue("Script_PahawhHmong", QChar::Script::Script_PahawhHmong);
  script.addValue("Script_Khojki", QChar::Script::Script_Khojki);
  script.addValue("Script_LinearA", QChar::Script::Script_LinearA);
  script.addValue("Script_Mahajani", QChar::Script::Script_Mahajani);
  script.addValue("Script_Manichaean", QChar::Script::Script_Manichaean);
  script.addValue("Script_MendeKikakui", QChar::Script::Script_MendeKikakui);
  script.addValue("Script_Modi", QChar::Script::Script_Modi);
  script.addValue("Script_Mro", QChar::Script::Script_Mro);
  script.addValue("Script_OldNorthArabian", QChar::Script::Script_OldNorthArabian);
  script.addValue("Script_Nabataean", QChar::Script::Script_Nabataean);
  script.addValue("Script_Palmyrene", QChar::Script::Script_Palmyrene);
  script.addValue("Script_PauCinHau", QChar::Script::Script_PauCinHau);
  script.addValue("Script_OldPermic", QChar::Script::Script_OldPermic);
  script.addValue("Script_PsalterPahlavi", QChar::Script::Script_PsalterPahlavi);
  script.addValue("Script_Siddham", QChar::Script::Script_Siddham);
  script.addValue("Script_Khudawadi", QChar::Script::Script_Khudawadi);
  script.addValue("Script_Tirhuta", QChar::Script::Script_Tirhuta);
  script.addValue("Script_WarangCiti", QChar::Script::Script_WarangCiti);
  script.addValue("Script_Ahom", QChar::Script::Script_Ahom);
  script.addValue("Script_AnatolianHieroglyphs", QChar::Script::Script_AnatolianHieroglyphs);
  script.addValue("Script_Hatran", QChar::Script::Script_Hatran);
  script.addValue("Script_Multani", QChar::Script::Script_Multani);
  script.addValue("Script_OldHungarian", QChar::Script::Script_OldHungarian);
  script.addValue("Script_SignWriting", QChar::Script::Script_SignWriting);
  script.addValue("Script_Adlam", QChar::Script::Script_Adlam);
  script.addValue("Script_Bhaiksuki", QChar::Script::Script_Bhaiksuki);
  script.addValue("Script_Marchen", QChar::Script::Script_Marchen);
  script.addValue("Script_Newa", QChar::Script::Script_Newa);
  script.addValue("Script_Osage", QChar::Script::Script_Osage);
  script.addValue("Script_Tangut", QChar::Script::Script_Tangut);
  script.addValue("Script_MasaramGondi", QChar::Script::Script_MasaramGondi);
  script.addValue("Script_Nushu", QChar::Script::Script_Nushu);
  script.addValue("Script_Soyombo", QChar::Script::Script_Soyombo);
  script.addValue("Script_ZanabazarSquare", QChar::Script::Script_ZanabazarSquare);
  script.addValue("Script_Dogra", QChar::Script::Script_Dogra);
  script.addValue("Script_GunjalaGondi", QChar::Script::Script_GunjalaGondi);
  script.addValue("Script_HanifiRohingya", QChar::Script::Script_HanifiRohingya);
  script.addValue("Script_Makasar", QChar::Script::Script_Makasar);
  script.addValue("Script_Medefaidrin", QChar::Script::Script_Medefaidrin);
  script.addValue("Script_OldSogdian", QChar::Script::Script_OldSogdian);
  script.addValue("Script_Sogdian", QChar::Script::Script_Sogdian);
  script.addValue("Script_Elymaic", QChar::Script::Script_Elymaic);
  script.addValue("Script_Nandinagari", QChar::Script::Script_Nandinagari);
  script.addValue("Script_NyiakengPuachueHmong", QChar::Script::Script_NyiakengPuachueHmong);
  script.addValue("Script_Wancho", QChar::Script::Script_Wancho);
  script.addValue("Script_Chorasmian", QChar::Script::Script_Chorasmian);
  script.addValue("Script_DivesAkuru", QChar::Script::Script_DivesAkuru);
  script.addValue("Script_KhitanSmallScript", QChar::Script::Script_KhitanSmallScript);
  script.addValue("Script_Yezidi", QChar::Script::Script_Yezidi);
  script.addValue("ScriptCount", QChar::Script::ScriptCount);
}
static void register_char__direction(script::Class& parent)
{
  using namespace script;
  
  Enum direction = gonk::bind::enumeration<QChar::Direction>(parent, "Direction").get();

  direction.addValue("DirL", QChar::Direction::DirL);
  direction.addValue("DirR", QChar::Direction::DirR);
  direction.addValue("DirEN", QChar::Direction::DirEN);
  direction.addValue("DirES", QChar::Direction::DirES);
  direction.addValue("DirET", QChar::Direction::DirET);
  direction.addValue("DirAN", QChar::Direction::DirAN);
  direction.addValue("DirCS", QChar::Direction::DirCS);
  direction.addValue("DirB", QChar::Direction::DirB);
  direction.addValue("DirS", QChar::Direction::DirS);
  direction.addValue("DirWS", QChar::Direction::DirWS);
  direction.addValue("DirON", QChar::Direction::DirON);
  direction.addValue("DirLRE", QChar::Direction::DirLRE);
  direction.addValue("DirLRO", QChar::Direction::DirLRO);
  direction.addValue("DirAL", QChar::Direction::DirAL);
  direction.addValue("DirRLE", QChar::Direction::DirRLE);
  direction.addValue("DirRLO", QChar::Direction::DirRLO);
  direction.addValue("DirPDF", QChar::Direction::DirPDF);
  direction.addValue("DirNSM", QChar::Direction::DirNSM);
  direction.addValue("DirBN", QChar::Direction::DirBN);
  direction.addValue("DirLRI", QChar::Direction::DirLRI);
  direction.addValue("DirRLI", QChar::Direction::DirRLI);
  direction.addValue("DirFSI", QChar::Direction::DirFSI);
  direction.addValue("DirPDI", QChar::Direction::DirPDI);
}
static void register_char__decomposition(script::Class& parent)
{
  using namespace script;
  
  Enum decomposition = gonk::bind::enumeration<QChar::Decomposition>(parent, "Decomposition").get();

  decomposition.addValue("NoDecomposition", QChar::Decomposition::NoDecomposition);
  decomposition.addValue("Canonical", QChar::Decomposition::Canonical);
  decomposition.addValue("Font", QChar::Decomposition::Font);
  decomposition.addValue("NoBreak", QChar::Decomposition::NoBreak);
  decomposition.addValue("Initial", QChar::Decomposition::Initial);
  decomposition.addValue("Medial", QChar::Decomposition::Medial);
  decomposition.addValue("Final", QChar::Decomposition::Final);
  decomposition.addValue("Isolated", QChar::Decomposition::Isolated);
  decomposition.addValue("Circle", QChar::Decomposition::Circle);
  decomposition.addValue("Super", QChar::Decomposition::Super);
  decomposition.addValue("Sub", QChar::Decomposition::Sub);
  decomposition.addValue("Vertical", QChar::Decomposition::Vertical);
  decomposition.addValue("Wide", QChar::Decomposition::Wide);
  decomposition.addValue("Narrow", QChar::Decomposition::Narrow);
  decomposition.addValue("Small", QChar::Decomposition::Small);
  decomposition.addValue("Square", QChar::Decomposition::Square);
  decomposition.addValue("Compat", QChar::Decomposition::Compat);
  decomposition.addValue("Fraction", QChar::Decomposition::Fraction);
}
static void register_char__joining_type(script::Class& parent)
{
  using namespace script;
  
  Enum joining_type = gonk::bind::enumeration<QChar::JoiningType>(parent, "JoiningType").get();

  joining_type.addValue("Joining_None", QChar::JoiningType::Joining_None);
  joining_type.addValue("Joining_Causing", QChar::JoiningType::Joining_Causing);
  joining_type.addValue("Joining_Dual", QChar::JoiningType::Joining_Dual);
  joining_type.addValue("Joining_Right", QChar::JoiningType::Joining_Right);
  joining_type.addValue("Joining_Left", QChar::JoiningType::Joining_Left);
  joining_type.addValue("Joining_Transparent", QChar::JoiningType::Joining_Transparent);
}
static void register_char__joining(script::Class& parent)
{
  using namespace script;
  
  Enum joining = gonk::bind::enumeration<QChar::Joining>(parent, "Joining").get();

  joining.addValue("OtherJoining", QChar::Joining::OtherJoining);
  joining.addValue("Dual", QChar::Joining::Dual);
  joining.addValue("Right", QChar::Joining::Right);
  joining.addValue("Center", QChar::Joining::Center);
}
static void register_char__combining_class(script::Class& parent)
{
  using namespace script;
  
  Enum combining_class = gonk::bind::enumeration<QChar::CombiningClass>(parent, "CombiningClass").get();

  combining_class.addValue("Combining_BelowLeftAttached", QChar::CombiningClass::Combining_BelowLeftAttached);
  combining_class.addValue("Combining_BelowAttached", QChar::CombiningClass::Combining_BelowAttached);
  combining_class.addValue("Combining_BelowRightAttached", QChar::CombiningClass::Combining_BelowRightAttached);
  combining_class.addValue("Combining_LeftAttached", QChar::CombiningClass::Combining_LeftAttached);
  combining_class.addValue("Combining_RightAttached", QChar::CombiningClass::Combining_RightAttached);
  combining_class.addValue("Combining_AboveLeftAttached", QChar::CombiningClass::Combining_AboveLeftAttached);
  combining_class.addValue("Combining_AboveAttached", QChar::CombiningClass::Combining_AboveAttached);
  combining_class.addValue("Combining_AboveRightAttached", QChar::CombiningClass::Combining_AboveRightAttached);
  combining_class.addValue("Combining_BelowLeft", QChar::CombiningClass::Combining_BelowLeft);
  combining_class.addValue("Combining_Below", QChar::CombiningClass::Combining_Below);
  combining_class.addValue("Combining_BelowRight", QChar::CombiningClass::Combining_BelowRight);
  combining_class.addValue("Combining_Left", QChar::CombiningClass::Combining_Left);
  combining_class.addValue("Combining_Right", QChar::CombiningClass::Combining_Right);
  combining_class.addValue("Combining_AboveLeft", QChar::CombiningClass::Combining_AboveLeft);
  combining_class.addValue("Combining_Above", QChar::CombiningClass::Combining_Above);
  combining_class.addValue("Combining_AboveRight", QChar::CombiningClass::Combining_AboveRight);
  combining_class.addValue("Combining_DoubleBelow", QChar::CombiningClass::Combining_DoubleBelow);
  combining_class.addValue("Combining_DoubleAbove", QChar::CombiningClass::Combining_DoubleAbove);
  combining_class.addValue("Combining_IotaSubscript", QChar::CombiningClass::Combining_IotaSubscript);
}
static void register_char__unicode_version(script::Class& parent)
{
  using namespace script;
  
  Enum unicode_version = gonk::bind::enumeration<QChar::UnicodeVersion>(parent, "UnicodeVersion").get();

  unicode_version.addValue("Unicode_Unassigned", QChar::UnicodeVersion::Unicode_Unassigned);
  unicode_version.addValue("Unicode_1_1", QChar::UnicodeVersion::Unicode_1_1);
  unicode_version.addValue("Unicode_2_0", QChar::UnicodeVersion::Unicode_2_0);
  unicode_version.addValue("Unicode_2_1_2", QChar::UnicodeVersion::Unicode_2_1_2);
  unicode_version.addValue("Unicode_3_0", QChar::UnicodeVersion::Unicode_3_0);
  unicode_version.addValue("Unicode_3_1", QChar::UnicodeVersion::Unicode_3_1);
  unicode_version.addValue("Unicode_3_2", QChar::UnicodeVersion::Unicode_3_2);
  unicode_version.addValue("Unicode_4_0", QChar::UnicodeVersion::Unicode_4_0);
  unicode_version.addValue("Unicode_4_1", QChar::UnicodeVersion::Unicode_4_1);
  unicode_version.addValue("Unicode_5_0", QChar::UnicodeVersion::Unicode_5_0);
  unicode_version.addValue("Unicode_5_1", QChar::UnicodeVersion::Unicode_5_1);
  unicode_version.addValue("Unicode_5_2", QChar::UnicodeVersion::Unicode_5_2);
  unicode_version.addValue("Unicode_6_0", QChar::UnicodeVersion::Unicode_6_0);
  unicode_version.addValue("Unicode_6_1", QChar::UnicodeVersion::Unicode_6_1);
  unicode_version.addValue("Unicode_6_2", QChar::UnicodeVersion::Unicode_6_2);
  unicode_version.addValue("Unicode_6_3", QChar::UnicodeVersion::Unicode_6_3);
  unicode_version.addValue("Unicode_7_0", QChar::UnicodeVersion::Unicode_7_0);
  unicode_version.addValue("Unicode_8_0", QChar::UnicodeVersion::Unicode_8_0);
  unicode_version.addValue("Unicode_9_0", QChar::UnicodeVersion::Unicode_9_0);
  unicode_version.addValue("Unicode_10_0", QChar::UnicodeVersion::Unicode_10_0);
  unicode_version.addValue("Unicode_11_0", QChar::UnicodeVersion::Unicode_11_0);
  unicode_version.addValue("Unicode_12_0", QChar::UnicodeVersion::Unicode_12_0);
  unicode_version.addValue("Unicode_12_1", QChar::UnicodeVersion::Unicode_12_1);
  unicode_version.addValue("Unicode_13_0", QChar::UnicodeVersion::Unicode_13_0);
}

static void register_char_(script::Namespace& parent)
{
  using namespace script;
  
  Class char_ = parent.newClass("QChar").setId(script::Type::make<QChar>().data())
    .get();

  Class& c = char_;
  
  register_char__special_character(c);
          register_char__category(c);
          register_char__script(c);
          register_char__direction(c);
          register_char__decomposition(c);
          register_char__joining_type(c);
          register_char__joining(c);
          register_char__combining_class(c);
          register_char__unicode_version(c);
        
  // QChar();
  gonk::bind::default_constructor<QChar>(c).create();
  // QChar(ushort rc);
  /// TODO: QChar(ushort rc);
  // QChar(uchar c, uchar r);
  /// TODO: QChar(uchar c, uchar r);
  // QChar(short rc);
  /// TODO: QChar(short rc);
  // QChar(uint rc);
  /// TODO: QChar(uint rc);
  // QChar(int rc);
  gonk::bind::constructor<QChar, int>(c).create();
  // QChar(QChar::SpecialCharacter s);
  gonk::bind::constructor<QChar, QChar::SpecialCharacter>(c).create();
  // QChar(QLatin1Char ch);
  gonk::bind::constructor<QChar, QLatin1Char>(c).create();
  // QChar(char16_t ch);
  /// TODO: QChar(char16_t ch);
  // QChar(wchar_t ch);
  /// TODO: QChar(wchar_t ch);
  // QChar(char c);
  gonk::bind::constructor<QChar, char>(c).create();
  // QChar(uchar c);
  /// TODO: QChar(uchar c);
  // QChar::Category category() const;
  gonk::bind::member_function<QChar, QChar::Category, &QChar::category>(c, "category").create();
  // QChar::Direction direction() const;
  gonk::bind::member_function<QChar, QChar::Direction, &QChar::direction>(c, "direction").create();
  // QChar::JoiningType joiningType() const;
  gonk::bind::member_function<QChar, QChar::JoiningType, &QChar::joiningType>(c, "joiningType").create();
  // QChar::Joining joining() const;
  gonk::bind::member_function<QChar, QChar::Joining, &QChar::joining>(c, "joining").create();
  // unsigned char combiningClass() const;
  /// TODO: unsigned char combiningClass() const;
  // QChar mirroredChar() const;
  gonk::bind::member_function<QChar, QChar, &QChar::mirroredChar>(c, "mirroredChar").create();
  // bool hasMirrored() const;
  gonk::bind::member_function<QChar, bool, &QChar::hasMirrored>(c, "hasMirrored").create();
  // QString decomposition() const;
  gonk::bind::member_function<QChar, QString, &QChar::decomposition>(c, "decomposition").create();
  // QChar::Decomposition decompositionTag() const;
  gonk::bind::member_function<QChar, QChar::Decomposition, &QChar::decompositionTag>(c, "decompositionTag").create();
  // int digitValue() const;
  gonk::bind::member_function<QChar, int, &QChar::digitValue>(c, "digitValue").create();
  // QChar toLower() const;
  gonk::bind::member_function<QChar, QChar, &QChar::toLower>(c, "toLower").create();
  // QChar toUpper() const;
  gonk::bind::member_function<QChar, QChar, &QChar::toUpper>(c, "toUpper").create();
  // QChar toTitleCase() const;
  gonk::bind::member_function<QChar, QChar, &QChar::toTitleCase>(c, "toTitleCase").create();
  // QChar toCaseFolded() const;
  gonk::bind::member_function<QChar, QChar, &QChar::toCaseFolded>(c, "toCaseFolded").create();
  // QChar::Script script() const;
  gonk::bind::member_function<QChar, QChar::Script, &QChar::script>(c, "script").create();
  // QChar::UnicodeVersion unicodeVersion() const;
  gonk::bind::member_function<QChar, QChar::UnicodeVersion, &QChar::unicodeVersion>(c, "unicodeVersion").create();
  // char toLatin1() const;
  gonk::bind::member_function<QChar, char, &QChar::toLatin1>(c, "toLatin1").create();
  // ushort unicode() const;
  /// TODO: ushort unicode() const;
  // ushort& unicode();
  /// TODO: ushort& unicode();
  // static QChar fromLatin1(char c);
  gonk::bind::static_member_function<QChar, QChar, char, &QChar::fromLatin1>(c, "fromLatin1").create();
  // bool isNull() const;
  gonk::bind::member_function<QChar, bool, &QChar::isNull>(c, "isNull").create();
  // bool isPrint() const;
  gonk::bind::member_function<QChar, bool, &QChar::isPrint>(c, "isPrint").create();
  // bool isSpace() const;
  gonk::bind::member_function<QChar, bool, &QChar::isSpace>(c, "isSpace").create();
  // bool isMark() const;
  gonk::bind::member_function<QChar, bool, &QChar::isMark>(c, "isMark").create();
  // bool isPunct() const;
  gonk::bind::member_function<QChar, bool, &QChar::isPunct>(c, "isPunct").create();
  // bool isSymbol() const;
  gonk::bind::member_function<QChar, bool, &QChar::isSymbol>(c, "isSymbol").create();
  // bool isLetter() const;
  gonk::bind::member_function<QChar, bool, &QChar::isLetter>(c, "isLetter").create();
  // bool isNumber() const;
  gonk::bind::member_function<QChar, bool, &QChar::isNumber>(c, "isNumber").create();
  // bool isLetterOrNumber() const;
  gonk::bind::member_function<QChar, bool, &QChar::isLetterOrNumber>(c, "isLetterOrNumber").create();
  // bool isDigit() const;
  gonk::bind::member_function<QChar, bool, &QChar::isDigit>(c, "isDigit").create();
  // bool isLower() const;
  gonk::bind::member_function<QChar, bool, &QChar::isLower>(c, "isLower").create();
  // bool isUpper() const;
  gonk::bind::member_function<QChar, bool, &QChar::isUpper>(c, "isUpper").create();
  // bool isTitleCase() const;
  gonk::bind::member_function<QChar, bool, &QChar::isTitleCase>(c, "isTitleCase").create();
  // bool isNonCharacter() const;
  gonk::bind::member_function<QChar, bool, &QChar::isNonCharacter>(c, "isNonCharacter").create();
  // bool isHighSurrogate() const;
  gonk::bind::member_function<QChar, bool, &QChar::isHighSurrogate>(c, "isHighSurrogate").create();
  // bool isLowSurrogate() const;
  gonk::bind::member_function<QChar, bool, &QChar::isLowSurrogate>(c, "isLowSurrogate").create();
  // bool isSurrogate() const;
  gonk::bind::member_function<QChar, bool, &QChar::isSurrogate>(c, "isSurrogate").create();
  // uchar cell() const;
  /// TODO: uchar cell() const;
  // uchar row() const;
  /// TODO: uchar row() const;
  // void setCell(uchar acell);
  /// TODO: void setCell(uchar acell);
  // void setRow(uchar arow);
  /// TODO: void setRow(uchar arow);
  // static bool isNonCharacter(uint ucs4);
  /// TODO: static bool isNonCharacter(uint ucs4);
  // static bool isHighSurrogate(uint ucs4);
  /// TODO: static bool isHighSurrogate(uint ucs4);
  // static bool isLowSurrogate(uint ucs4);
  /// TODO: static bool isLowSurrogate(uint ucs4);
  // static bool isSurrogate(uint ucs4);
  /// TODO: static bool isSurrogate(uint ucs4);
  // static bool requiresSurrogates(uint ucs4);
  /// TODO: static bool requiresSurrogates(uint ucs4);
  // static uint surrogateToUcs4(ushort high, ushort low);
  /// TODO: static uint surrogateToUcs4(ushort high, ushort low);
  // static uint surrogateToUcs4(QChar high, QChar low);
  /// TODO: static uint surrogateToUcs4(QChar high, QChar low);
  // static ushort highSurrogate(uint ucs4);
  /// TODO: static ushort highSurrogate(uint ucs4);
  // static ushort lowSurrogate(uint ucs4);
  /// TODO: static ushort lowSurrogate(uint ucs4);
  // static QChar::Category category(uint ucs4);
  /// TODO: static QChar::Category category(uint ucs4);
  // static QChar::Direction direction(uint ucs4);
  /// TODO: static QChar::Direction direction(uint ucs4);
  // static QChar::JoiningType joiningType(uint ucs4);
  /// TODO: static QChar::JoiningType joiningType(uint ucs4);
  // static QChar::Joining joining(uint ucs4);
  /// TODO: static QChar::Joining joining(uint ucs4);
  // static unsigned char combiningClass(uint ucs4);
  /// TODO: static unsigned char combiningClass(uint ucs4);
  // static uint mirroredChar(uint ucs4);
  /// TODO: static uint mirroredChar(uint ucs4);
  // static bool hasMirrored(uint ucs4);
  /// TODO: static bool hasMirrored(uint ucs4);
  // static QString decomposition(uint ucs4);
  /// TODO: static QString decomposition(uint ucs4);
  // static QChar::Decomposition decompositionTag(uint ucs4);
  /// TODO: static QChar::Decomposition decompositionTag(uint ucs4);
  // static int digitValue(uint ucs4);
  /// TODO: static int digitValue(uint ucs4);
  // static uint toLower(uint ucs4);
  /// TODO: static uint toLower(uint ucs4);
  // static uint toUpper(uint ucs4);
  /// TODO: static uint toUpper(uint ucs4);
  // static uint toTitleCase(uint ucs4);
  /// TODO: static uint toTitleCase(uint ucs4);
  // static uint toCaseFolded(uint ucs4);
  /// TODO: static uint toCaseFolded(uint ucs4);
  // static QChar::Script script(uint ucs4);
  /// TODO: static QChar::Script script(uint ucs4);
  // static QChar::UnicodeVersion unicodeVersion(uint ucs4);
  /// TODO: static QChar::UnicodeVersion unicodeVersion(uint ucs4);
  // static QChar::UnicodeVersion currentUnicodeVersion();
  gonk::bind::static_member_function<QChar, QChar::UnicodeVersion, &QChar::currentUnicodeVersion>(c, "currentUnicodeVersion").create();
  // static bool isPrint(uint ucs4);
  /// TODO: static bool isPrint(uint ucs4);
  // static bool isSpace(uint ucs4);
  /// TODO: static bool isSpace(uint ucs4);
  // static bool isMark(uint ucs4);
  /// TODO: static bool isMark(uint ucs4);
  // static bool isPunct(uint ucs4);
  /// TODO: static bool isPunct(uint ucs4);
  // static bool isSymbol(uint ucs4);
  /// TODO: static bool isSymbol(uint ucs4);
  // static bool isLetter(uint ucs4);
  /// TODO: static bool isLetter(uint ucs4);
  // static bool isNumber(uint ucs4);
  /// TODO: static bool isNumber(uint ucs4);
  // static bool isLetterOrNumber(uint ucs4);
  /// TODO: static bool isLetterOrNumber(uint ucs4);
  // static bool isDigit(uint ucs4);
  /// TODO: static bool isDigit(uint ucs4);
  // static bool isLower(uint ucs4);
  /// TODO: static bool isLower(uint ucs4);
  // static bool isUpper(uint ucs4);
  /// TODO: static bool isUpper(uint ucs4);
  // static bool isTitleCase(uint ucs4);
  /// TODO: static bool isTitleCase(uint ucs4);
  // static bool isSpace_helper(uint ucs4);
  /// TODO: static bool isSpace_helper(uint ucs4);
  // static bool isLetter_helper(uint ucs4);
  /// TODO: static bool isLetter_helper(uint ucs4);
  // static bool isNumber_helper(uint ucs4);
  /// TODO: static bool isNumber_helper(uint ucs4);
  // static bool isLetterOrNumber_helper(uint ucs4);
  /// TODO: static bool isLetterOrNumber_helper(uint ucs4);
}
#endif // METAGONK_SOURCE

void register_char__file(script::Namespace ns)
{
  using namespace script;


  register_latin1_char(ns);
  register_char_(ns);

  // bool operator==(QChar, QChar);
  gonk::bind::op_eq<QChar, QChar>(ns);
  // bool operator<(QChar, QChar);
  gonk::bind::op_less<QChar, QChar>(ns);
  // bool operator!=(QChar, QChar);
  gonk::bind::op_neq<QChar, QChar>(ns);
  // bool operator>=(QChar, QChar);
  gonk::bind::op_geq<QChar, QChar>(ns);
  // bool operator>(QChar, QChar);
  gonk::bind::op_greater<QChar, QChar>(ns);
  // bool operator<=(QChar, QChar);
  gonk::bind::op_leq<QChar, QChar>(ns);
  // bool operator==(QChar, std::nullptr_t);
  /// TODO: bool operator==(QChar, std::nullptr_t);
  // bool operator<(QChar, std::nullptr_t);
  /// TODO: bool operator<(QChar, std::nullptr_t);
  // bool operator==(std::nullptr_t, QChar);
  /// TODO: bool operator==(std::nullptr_t, QChar);
  // bool operator<(std::nullptr_t, QChar);
  /// TODO: bool operator<(std::nullptr_t, QChar);
  // bool operator!=(QChar, std::nullptr_t);
  /// TODO: bool operator!=(QChar, std::nullptr_t);
  // bool operator>=(QChar, std::nullptr_t);
  /// TODO: bool operator>=(QChar, std::nullptr_t);
  // bool operator>(QChar, std::nullptr_t);
  /// TODO: bool operator>(QChar, std::nullptr_t);
  // bool operator<=(QChar, std::nullptr_t);
  /// TODO: bool operator<=(QChar, std::nullptr_t);
  // bool operator!=(std::nullptr_t, QChar);
  /// TODO: bool operator!=(std::nullptr_t, QChar);
  // bool operator>=(std::nullptr_t, QChar);
  /// TODO: bool operator>=(std::nullptr_t, QChar);
  // bool operator>(std::nullptr_t, QChar);
  /// TODO: bool operator>(std::nullptr_t, QChar);
  // bool operator<=(std::nullptr_t, QChar);
  /// TODO: bool operator<=(std::nullptr_t, QChar);
  // QDataStream& operator<<(QDataStream&, QChar);
  /// TODO: QDataStream& operator<<(QDataStream&, QChar);
  // QDataStream& operator>>(QDataStream&, QChar&);
  /// TODO: QDataStream& operator>>(QDataStream&, QChar&);
}
