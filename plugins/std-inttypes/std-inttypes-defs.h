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

} // namespace std_inttypes

} // namespace gonk

#endif // GONK_STD_INTTYPES_DEFS_H