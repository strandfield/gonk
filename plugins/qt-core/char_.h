// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_QT_CORE_CHAR__H
#define GONK_QT_CORE_CHAR__H

#include "gonk/common/types.h"

#include "qt-core-defs.h"

#include <QChar>

namespace script
{
template<> struct make_type_helper<QLatin1Char> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QLatin1Char)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QChar::SpecialCharacter> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharSpecialCharacter)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::Category> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharCategory)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::Script> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharScript)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::Direction> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharDirection)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::Decomposition> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharDecomposition)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::JoiningType> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharJoiningType)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::Joining> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharJoining)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::CombiningClass> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharCombiningClass)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar::UnicodeVersion> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QCharUnicodeVersion)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QChar> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QChar)) | script::Type::ObjectFlag; } };
} // namespace script

#endif // GONK_QT_CORE_CHAR__H
