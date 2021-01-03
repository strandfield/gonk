// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_GONK_DEBUGGER_H
#define GONK_GONK_DEBUGGER_H

#include "gonk-debugger-defs.h"

#include "gonk/plugin.h"

extern "C"
{

  GONK_GONK_DEBUGGER_API gonk::Plugin* gonk_debugger_module();

} // extern "C"

#endif // GONK_GONK_DEBUGGER_H
