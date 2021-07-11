// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_MAP_H
#define GONK_STD_MAP_H

#include "std-map-defs.h"

#include "gonk/plugin.h"

extern "C"
{

  GONK_STD_MAP_API gonk::Plugin* gonk_std_map_module();

} // extern "C"

#endif // GONK_STD_MAP_H
