// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_COMPILE_LIBRARY)
#  define GONK_API __declspec(dllexport)
#else
#  define GONK_API __declspec(dllimport)
#endif
#else
#define GONK_API
#endif
