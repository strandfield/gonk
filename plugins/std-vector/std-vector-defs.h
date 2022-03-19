// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_VECTOR_DEFS_H
#define GONK_STD_VECTOR_DEFS_H

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_STD_VECTOR_COMPILE_LIBRARY)
#  define GONK_STD_VECTOR_API __declspec(dllexport)
#else
#  define GONK_STD_VECTOR_API __declspec(dllimport)
#endif
#else
#define GONK_STD_VECTOR_API
#endif

namespace gonk
{

namespace std_vector
{

} // namespace std_vector

} // namespace gonk

#endif // GONK_STD_VECTOR_DEFS_H