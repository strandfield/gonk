// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

// Warning: This file was generated by metagonk, all modifications will be lost. //

#ifndef GONK_QT_CORE_DEFS_H
#define GONK_QT_CORE_DEFS_H

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_QT_CORE_COMPILE_LIBRARY)
#  define GONK_QT_CORE_API __declspec(dllexport)
#else
#  define GONK_QT_CORE_API __declspec(dllimport)
#endif
#else
#define GONK_QT_CORE_API
#endif

namespace gonk
{

namespace qt_core
{

#if METAGONK_SOURCE
{% include generate_typeid_list with types = project.types %}{% newline %}
#else
enum class EnumTypeIds
{
  FirstTypeId,
  QByteArrayBase64Option,
  QByteArrayBase64DecodingStatus,
  QCharSpecialCharacter,
  QCharCategory,
  QCharScript,
  QCharDirection,
  QCharDecomposition,
  QCharJoiningType,
  QCharJoining,
  QCharCombiningClass,
  QCharUnicodeVersion,
  QStringSectionFlag,
  QStringSplitBehavior,
  QStringNormalizationForm,
  QDirFilter,
  QDirSortFlag,
  LastTypeId,
};

enum class ClassTypeIds
{
  FirstTypeId,
  QByteArrayFromBase64Result,
  QByteArray,
  QByteRef,
  FromBase64Result,
  QLatin1Char,
  QChar,
  QLatin1String,
  QStringNull,
  QString,
  QCharRef,
  QStringRef,
  QDir,
  LastTypeId,
};
#endif // METAGONK_SOURCE

GONK_QT_CORE_API int enum_type_id_offset();
GONK_QT_CORE_API int class_type_id_offset();

} // namespace qt_core

} // namespace gonk

#endif // GONK_QT_CORE_DEFS_H
