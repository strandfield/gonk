// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_QT_CORE_BYTEARRAY_H
#define GONK_QT_CORE_BYTEARRAY_H

#include "gonk/common/types.h"

#include "qt-core-defs.h"

#include <QByteArray>

namespace script
{
#if METAGONK_SOURCE
{% assign bytearray_class = project | get_symbol: 'Qt.Core', 'QByteArray'%}
{% include generate_make_type_helpers with entity = bytearray_class and module_namespace = 'qt_core' and recursive = true %}
{% assign byteref_class = project | get_symbol: 'Qt.Core', 'QByteRef' %}
{% include generate_make_type_helpers with entity = byteref_class and module_namespace = 'qt_core' and recursive = true %}
#else
template<> struct make_type_helper<QByteArray::Base64Option> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QByteArrayBase64Option)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QByteArray::Base64DecodingStatus> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QByteArrayBase64DecodingStatus)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QByteArray::FromBase64Result> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QByteArrayFromBase64Result)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QByteArray> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QByteArray)) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<QByteRef> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QByteRef)) | script::Type::ObjectFlag; } };
#endif // METAGONK_SOURCE
} // namespace script
#endif // GONK_QT_CORE_BYTEARRAY_H
