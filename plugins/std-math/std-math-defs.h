// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_MATH_DEFS_H
#define GONK_STD_MATH_DEFS_H

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_STD_MATH_COMPILE_LIBRARY)
#  define GONK_STD_MATH_API __declspec(dllexport)
#else
#  define GONK_STD_MATH_API __declspec(dllimport)
#endif
#else
#define GONK_STD_MATH_API
#endif

namespace gonk
{

namespace std_math
{

} // namespace std_math

} // namespace gonk

#endif // GONK_STD_MATH_DEFS_H