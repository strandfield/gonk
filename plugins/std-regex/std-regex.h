// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_REGEX_H
#define GONK_STD_REGEX_H

#include "std-regex-defs.h"

#include "gonk/plugin.h"

extern "C"
{

  GONK_STD_REGEX_API gonk::Plugin* gonk_std_regex_module();

} // extern "C"

#endif // GONK_STD_REGEX_H
