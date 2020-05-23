// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_REGEX_DEFS_H
#define GONK_STD_REGEX_DEFS_H

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_STD_REGEX_COMPILE_LIBRARY)
#  define GONK_STD_REGEX_API __declspec(dllexport)
#else
#  define GONK_STD_REGEX_API __declspec(dllimport)
#endif
#else
#define GONK_STD_REGEX_API
#endif

namespace gonk
{

namespace std_regex
{

enum ClassTypeIds
{
  Regex,
};

GONK_STD_REGEX_API int class_type_id_offset();
GONK_STD_REGEX_API int enum_type_id_offset();

} // namespace std_regex

} // namespace gonk

#endif // GONK_STD_REGEX_DEFS_H