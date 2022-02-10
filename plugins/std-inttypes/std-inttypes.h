// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_INTTYPES_H
#define GONK_STD_INTTYPES_H

#include "std-inttypes-defs.h"

#include "gonk/common/types.h"

#include <cstdint>

#if UINT32_MAX == 4294967295
#define GONK_INT32_IS_INT
#endif

namespace gonk
{
class Plugin;
} // namespace gonk

extern "C"
{

  GONK_STD_INTTYPES_API gonk::Plugin* gonk_std_inttypes_module();

} // extern "C"

#endif // GONK_STD_INTTYPES_H
