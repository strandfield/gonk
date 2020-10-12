// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_INTTYPES_DEFS_H
#define GONK_STD_INTTYPES_DEFS_H

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_STD_INTTYPES_COMPILE_LIBRARY)
#  define GONK_STD_INTTYPES_API __declspec(dllexport)
#else
#  define GONK_STD_INTTYPES_API __declspec(dllimport)
#endif
#else
#define GONK_STD_INTTYPES_API
#endif

namespace gonk
{

namespace std_inttypes
{

enum ClassTypeIds
{
  Int32,
  Int64,
  UInt32,
  UInt64,
};

GONK_STD_INTTYPES_API int class_type_id_offset();
GONK_STD_INTTYPES_API int enum_type_id_offset();

} // namespace std_inttypes

} // namespace gonk

#endif // GONK_STD_INTTYPES_DEFS_H