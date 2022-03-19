// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_TESTHYBRIDMODULE_DEFS_H
#define GONK_TESTHYBRIDMODULE_DEFS_H

#if (defined(WIN32) || defined(_WIN32))
#if defined(GONK_GONKTESTHYBRIDMODULE_COMPILE_LIBRARY)
#  define GONK_TESTHYBRIDMODULE_API __declspec(dllexport)
#else
#  define GONK_TESTHYBRIDMODULE_API __declspec(dllimport)
#endif
#else
#define GONK_TESTHYBRIDMODULE_API
#endif

#endif // GONK_TESTHYBRIDMODULE_DEFS_H