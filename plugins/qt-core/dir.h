// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_QT_CORE_DIR_H
#define GONK_QT_CORE_GONK_QT_CORE_DIR_HSTRING_H

#include "gonk/common/types.h"

#include "qt-core-defs.h"

#include <QDir>

namespace script
{
#if METAGONK_SOURCE
{% assign current_class = project | get_symbol: 'QDir'%}
{% include generate_make_type_helpers with entity = current_class and module_namespace = 'qt_core' and recursive = true %}
#else
template<> struct make_type_helper<QDir::Filter> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QDirFilter)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QDir::SortFlag> { inline static script::Type get(){ return (gonk::qt_core::enum_type_id_offset() + static_cast<int>(gonk::qt_core::EnumTypeIds::QDirSortFlag)) | script::Type::EnumFlag; } };
template<> struct make_type_helper<QDir> { inline static script::Type get(){ return (gonk::qt_core::class_type_id_offset() + static_cast<int>(gonk::qt_core::ClassTypeIds::QDir)) | script::Type::ObjectFlag; } };
#endif // METAGONK_SOURCE
} // namespace script

#endif // GONK_QT_CORE_DIR_H
