// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

// Warning: This file was generated by metagonk, all modifications will be lost. //

#ifndef GONK_QT_CORE_STRING_H
#define GONK_QT_CORE_STRING_H

#include "gonk/common/types.h"

#include "qt-core-defs.h"

#include <QString>

namespace script
{
template<> struct make_type_helper<QLatin1String> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QLatin1String)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QString::SectionFlag> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QStringSectionFlag)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QString::SplitBehavior> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QStringSplitBehavior)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QString::NormalizationForm> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QStringNormalizationForm)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QString::Null> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QStringNull)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QString> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QString)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QCharRef> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QCharRef)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QStringRef> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QStringRef)) | script::Type::ObjectFlag; } };
} // namespace script
#endif // GONK_QT_CORE_STRING_H