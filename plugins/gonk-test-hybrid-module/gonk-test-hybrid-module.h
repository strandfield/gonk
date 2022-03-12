// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_TESTHYBRIDMODULE_H
#define GONK_TESTHYBRIDMODULE_H

#include "gonk-test-hybrid-module-defs.h"

#include "gonk/plugin.h"

extern "C"
{

  GONK_TESTHYBRIDMODULE_API gonk::Plugin* gonk_test_hybrid_module();

} // extern "C"

#endif // GONK_TESTHYBRIDMODULE_H
